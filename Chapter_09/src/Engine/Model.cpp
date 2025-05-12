#include "Model.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Game.h"
#include "Mesh.h"
#include "Texture.h"
#include "Renderer.h"

namespace jLab
{
	Model::Model(const std::string& filename, Game* game)
	{
		m_Game = game;
		m_Directory = filename.substr(0, filename.find_last_of('/') + 1);
		Load(filename);
	}

	Model::~Model()
	{
		while (!m_Meshes.empty())
			delete m_Meshes.back();
	}

	void Model::Draw(const Shader* shader)
	{
		for (Mesh* mesh : m_Meshes)
			mesh->Draw(shader);
	}

	void Model::Load(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load model: %s", filename.c_str());
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

		// ----------------VERTICES-------------------
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vec3;

			vec3.x = mesh->mVertices[i].x;
			vec3.y = mesh->mVertices[i].y;
			vec3.z = mesh->mVertices[i].z;
			vertex.Position = vec3;

			vec3.x = mesh->mNormals[i].x;
			vec3.y = mesh->mNormals[i].y;
			vec3.z = mesh->mNormals[i].z;
			vertex.Normal = vec3;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec2;
				vec2.x = mesh->mTextureCoords[0][i].x;
				vec2.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoord = vec2;
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
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture*> diffuseTextures = LoadMaterialTexture(mat, aiTextureType_DIFFUSE, scene);
			textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

			std::vector<Texture*> specularTextures = LoadMaterialTexture(mat, aiTextureType_SPECULAR, scene);
			textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());

			std::vector<Texture*> normalTextures = LoadMaterialTexture(mat, aiTextureType_HEIGHT, scene);
			textures.insert(textures.end(), normalTextures.begin(), normalTextures.end());
		}

		return new Mesh(vertices, indices, textures);
	}

	std::vector<class Texture*> Model::LoadMaterialTexture(const aiMaterial* mat, const aiTextureType textureType, const aiScene* scene)
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
				texture = m_Game->GetRenderer()->GetTexture(m_Directory + textureName.C_Str(), Texture::E_Diffuse);
				break;
			case aiTextureType_SPECULAR:
				texture = m_Game->GetRenderer()->GetTexture(m_Directory + textureName.C_Str(), Texture::E_Specular);
				break;
			case aiTextureType_HEIGHT:
				texture = m_Game->GetRenderer()->GetTexture(m_Directory + textureName.C_Str(), Texture::E_Normal);
				break;
			default:
				break;
			}

			if (texture)
				textures.emplace_back(texture);
		}

		return textures;
	}
}