#include "Model.h"

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Skeleton.h"

namespace jLab
{
	Model::Model(const std::string& filename, Game* game, bool flipUvs, Skeleton* skeleton)
		:mGame(game)
		,mFlipUVs(flipUvs)
		,mSkeleton(skeleton)
		,mIsSkinned(false)
		,mAABB(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity()))
	{
		mDirectory = filename.substr(0, filename.find_last_of('/') + 1);

		if (mSkeleton)
			mIsSkinned = true;

		Load(filename);
		GenerateAABB();
	}

	Model::~Model()
	{
		while (!mMeshes.empty())
		{
			delete mMeshes.back();
			mMeshes.pop_back();
		}
	}

	void Model::Draw(const Shader* shader)
	{
		for (Mesh* mesh : mMeshes)
			mesh->Draw(shader);
	}
	
	void Model::Load(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_Triangulate);
		if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		{
			printf("Failed to load model '%s'", filename.c_str());
			return;
		}

		if (mIsSkinned)
		{
			std::vector<Skeleton::Bone> bones = mSkeleton->GetBones();
			for (int i = 0; i < bones.size(); i++)
				mBoneNameToIndexMap[bones[i].Name] = i;
		}

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			Mesh* mesh = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
			mMeshes.emplace_back(mesh);
		}

		for (int i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene);
	}

	Mesh* Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;

		// --------------------------VERTICES--------------------------
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.Position.x = mesh->mVertices[i].x;
			vertex.Position.y = mesh->mVertices[i].y;
			vertex.Position.z = mesh->mVertices[i].z;

			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
			}
			vertices.emplace_back(vertex);
		}

		// --------------------------INDICES---------------------------
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
				indices.emplace_back(face.mIndices[j]);
		}
		
		// --------------------------TEXTURES--------------------------
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture*> diffuseTextures = LoadTexturesFromAiMat(mat, aiTextureType_DIFFUSE, scene);
			textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

			std::vector<Texture*> specularTextures = LoadTexturesFromAiMat(mat, aiTextureType_SPECULAR, scene);
			textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

			std::vector<Texture*> normalTextures = LoadTexturesFromAiMat(mat, aiTextureType_HEIGHT, scene);
			textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
		}
		
		// --------------------------BONES-----------------------------
		if (mesh->mNumBones > 0 && mIsSkinned)
		{
			for (int i = 0; i < mesh->mNumBones; i++)
			{
				const aiBone* bone = mesh->mBones[i];
				uint8_t boneId = GetBoneId(bone);
				for (unsigned int j = 0; j < bone->mNumWeights; j++)
				{
					const aiVertexWeight& vw = bone->mWeights[j];
					unsigned int vertIndex = vw.mVertexId;
					uint8_t boneWeight = (uint8_t)std::round(vw.mWeight * 255);
					vertices[vertIndex].AddBone(boneId, boneWeight);
				}
			}
			return new Mesh(vertices, indices, textures, mIsSkinned);
		}

		return new Mesh(vertices, indices, textures);
	}

	std::vector<class Texture*> Model::LoadTexturesFromAiMat(const aiMaterial* material, aiTextureType textureType, const aiScene* scene)
	{
		std::vector<Texture*> textures;
		aiString textureName;

		for (int i = 0; i < material->GetTextureCount(textureType); i++)
		{
			Texture* texture = nullptr;
			material->GetTexture(textureType, i, &textureName);

			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(textureName.C_Str());
			if (embeddedTexture)
			{
				switch (textureType)
				{
				case aiTextureType_DIFFUSE:
					texture = mGame->GetRenderer()->GetTexture(mDirectory + textureName.C_Str(), embeddedTexture, mFlipUVs, Texture::Type::Diffuse);
					break;
				case aiTextureType_SPECULAR:
					texture = mGame->GetRenderer()->GetTexture(mDirectory + textureName.C_Str(), embeddedTexture, mFlipUVs, Texture::Type::Specular);
					break;
				case aiTextureType_HEIGHT:
					texture = mGame->GetRenderer()->GetTexture(mDirectory + textureName.C_Str(), embeddedTexture, mFlipUVs, Texture::Type::Normal);
					break;
				default:
					break;
				}
			}
			else
			{
				switch (textureType)
				{
				case aiTextureType_DIFFUSE:
					texture = mGame->GetRenderer()->GetTexture(mDirectory + textureName.C_Str(), mFlipUVs, Texture::Type::Diffuse);
					break;
				case aiTextureType_SPECULAR:
					texture = mGame->GetRenderer()->GetTexture(mDirectory + textureName.C_Str(), mFlipUVs, Texture::Type::Specular);
					break;
				case aiTextureType_HEIGHT:
					texture = mGame->GetRenderer()->GetTexture(mDirectory + textureName.C_Str(), mFlipUVs, Texture::Type::Normal);
					break;
				default:
					break;
				}
			}
			if (texture)
				textures.emplace_back(texture);
		}

		return textures;
	}

	unsigned int Model::GetBoneId(const aiBone* bone)
	{
		std::string boneName = bone->mName.C_Str();

		if (mBoneNameToIndexMap.find(boneName) != mBoneNameToIndexMap.end())
			return mBoneNameToIndexMap[boneName];
		else
			printf("Bone '%s' could not be found in the skeleton", boneName.c_str());

		return 0;
	}
	
	void Model::GenerateAABB()
	{
		for (Mesh* mesh : mMeshes)
		{
			const std::vector<Vertex> vertices = mesh->GetVertices();
			for (Vertex vertex : vertices)
				mAABB.UpdateMinMax(vertex.Position);
		}
	}
}