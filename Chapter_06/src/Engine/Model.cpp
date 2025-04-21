#include "Model.h"

#include <SDL/SDL.h>
#include "Game.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

namespace jLab
{
	Model::Model(const std::string& path, Game* game)
	{
		m_Game = game;
		m_Directory = path.substr(0, path.find_last_of('/') + 1);

		Load(path);
	}
	
	Model::~Model()
	{
	}
	
	void Model::Draw(const Shader* shader)
	{
		for (Mesh* mesh : m_Meshes)
			mesh->Draw(shader);
	}
	
	void Model::Load(const std::string& path)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			SDL_Log("ERROR: Failed to import model: %s", path.c_str());
			return;
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

		// ----------------VERTICES-----------------
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 tempVec3;

			tempVec3.x = mesh->mVertices[i].x;
			tempVec3.y = mesh->mVertices[i].y;
			tempVec3.z = mesh->mVertices[i].z;
			vertex.position = tempVec3;

			tempVec3.x = mesh->mNormals[i].x;
			tempVec3.y = mesh->mNormals[i].y;
			tempVec3.z = mesh->mNormals[i].z;
			vertex.normal = tempVec3;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 tempVec2;
				tempVec2.x = mesh->mTextureCoords[0][i].x;
				tempVec2.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = tempVec2;
			}
			vertices.push_back(vertex);
		}

		// ----------------INDICES-----------------
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// ----------------TEXTURES-----------------
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture*> diffuseTextures = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, scene);
			textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

			std::vector<Texture*> normalTextures = LoadMaterialTextures(mat, aiTextureType_HEIGHT, scene);
			textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());

			std::vector<Texture*> specularTextures = LoadMaterialTextures(mat, aiTextureType_SPECULAR, scene);
			textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
		}
		return new Mesh(vertices, indices, textures);
	}
	
	std::vector<class Texture*> Model::LoadMaterialTextures(const aiMaterial* mat, aiTextureType textureType, const aiScene* scene)
	{
		std::vector<Texture*> textures;
		aiString textureName;

		for (int i = 0; i < mat->GetTextureCount(textureType); i++)
		{
			Texture* texture = nullptr;
			mat->GetTexture(textureType, i, &textureName);

			switch (textureType)
			{
			case aiTextureType_DIFFUSE:
				texture = m_Game->GetTexture(m_Directory + textureName.C_Str(), Texture::DiffuseTexture);
				break;
			case aiTextureType_HEIGHT:
				texture = m_Game->GetTexture(m_Directory + textureName.C_Str(), Texture::NormalTexture);
				break;
			case aiTextureType_SPECULAR:
				texture = m_Game->GetTexture(m_Directory + textureName.C_Str(), Texture::SpecularTexture);
				break;
			default:
				break;
			}

			textures.emplace_back(texture);
		}

		return textures;
	}
}