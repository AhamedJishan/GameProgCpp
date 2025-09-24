#include "Model.h"

#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Skeleton.h"
#include <glm/glm.hpp>

namespace jLab
{
	Model::Model(const std::string& filename, Game* game, bool flipUVs, Skeleton* skeleton)
		:m_Game(game)
		,m_Skeleton(skeleton)
		,m_IsSkinned(false)
		,m_FlipUVs(flipUVs)
		,m_AABB(glm::vec3(std::numeric_limits<float>::infinity()), glm::vec3(-std::numeric_limits<float>::infinity()))
	{
		m_Directory = filename.substr(0, filename.find_last_of('/') + 1);

		if (m_Skeleton)
			m_IsSkinned = true;

		Load(filename);
		GenerateAABB();
	}
	
	Model::~Model()
	{
		while (!m_Meshes.empty())
		{
			delete m_Meshes.back();
			m_Meshes.pop_back();
		}
	}
	
	void Model::Draw(const Shader* shader)
	{
		for (Mesh* mesh : m_Meshes)
			mesh->Draw(shader);
	}
	
	void Model::Load(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load model: %s", filename.c_str());
			return;
		}

		if (m_IsSkinned)
		{
			std::vector<Skeleton::Bone> bones = m_Skeleton->GetBones();
			for (int i = 0; i < bones.size(); i++)
				m_BoneNameToIndexMapping[bones[i].m_Name] = i;
		}
		
		ProcessNode(scene->mRootNode, scene);
	}
	
	void Model::ProcessNode(const aiNode* node, const aiScene* scene)
	{
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			Mesh* mesh = ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
			m_Meshes.emplace_back(mesh);
		}

		for (int i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], scene);
	}
	
	Mesh* Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture*> textures;

		// ----------------VERTICES-------------------
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

		// ----------------INDICES-------------------
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
				indices.emplace_back(face.mIndices[j]);
		}

		// ----------------TEXTURES-------------------
		if (mesh->mMaterialIndex >=0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture*> diffuseTextures = LoadMaterialTexture(mat, aiTextureType_DIFFUSE, scene);
			textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

			std::vector<Texture*> normalTextures = LoadMaterialTexture(mat, aiTextureType_HEIGHT, scene);
			textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());

			std::vector<Texture*> specularTextures = LoadMaterialTexture(mat, aiTextureType_SPECULAR, scene);
			textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
		}

		// ----------------Bones-------------------
		if(mesh->mNumBones > 0 && m_IsSkinned)
		{
			for (unsigned int i = 0; i < mesh->mNumBones; i++)
			{
				const aiBone* bone = mesh->mBones[i];
				uint8_t boneId = GetBoneId(bone);

				for (unsigned int j = 0; j < bone->mNumWeights; j++)
				{
					aiVertexWeight& vw = bone->mWeights[j];
					unsigned int vertexIndex = vw.mVertexId;
					uint8_t weight = (uint8_t)std::round(vw.mWeight * 255);
					vertices[vertexIndex].AddBone(boneId, weight);
				}
			}
			return new Mesh(vertices, indices, textures, m_IsSkinned);
		}

		return new Mesh(vertices, indices, textures);
	}

	unsigned int Model::GetBoneId(const aiBone* bone)
	{
		unsigned int id = 0;
		std::string boneName = bone->mName.C_Str();

		if (m_BoneNameToIndexMapping.find(boneName) != m_BoneNameToIndexMapping.end())
			return m_BoneNameToIndexMapping[boneName];
		else
		{
			printf("%s could not be found in the skeleton", boneName.c_str());
		}

		return id;
	}
	
	std::vector<Texture*> Model::LoadMaterialTexture(const aiMaterial* mat, const aiTextureType textureType, const aiScene* scene)
	{
		std::vector<Texture*> textures;
		aiString textureName;

			for (int i = 0; i < mat->GetTextureCount(textureType); i++)
		{
			Texture* texture = nullptr;
			mat->GetTexture(textureType, i, &textureName);

			const aiTexture* embeddedTex = scene->GetEmbeddedTexture(textureName.C_Str());
			if (embeddedTex)
			{
				switch (textureType)
				{
				case aiTextureType_DIFFUSE:
					texture = m_Game->GetRenderer()->GetTexture(textureName.C_Str(), embeddedTex, m_FlipUVs, Texture::E_Diffuse);
					break;
				case aiTextureType_HEIGHT:
					texture = m_Game->GetRenderer()->GetTexture(textureName.C_Str(), embeddedTex, m_FlipUVs, Texture::E_Normal);
					break;
				case aiTextureType_SPECULAR:
					texture = m_Game->GetRenderer()->GetTexture(textureName.C_Str(), embeddedTex, m_FlipUVs, Texture::E_Specular);
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
					texture = m_Game->GetRenderer()->GetTexture(m_Directory + textureName.C_Str(), m_FlipUVs, Texture::E_Diffuse);
					break;
				case aiTextureType_HEIGHT:
					texture = m_Game->GetRenderer()->GetTexture(m_Directory + textureName.C_Str(), m_FlipUVs, Texture::E_Normal);
					break;
				case aiTextureType_SPECULAR:
					texture = m_Game->GetRenderer()->GetTexture(m_Directory + textureName.C_Str(), m_FlipUVs, Texture::E_Specular);
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

	void Model::GenerateAABB()
	{
		for (Mesh* mesh : m_Meshes)
		{
			std::vector<Vertex> vertices = mesh->GetVertices();
			for (Vertex& vertex : vertices)
				m_AABB.UpdateMinMax(vertex.Position);
		}
	}
}