#pragma once

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Collision.h"

namespace jLab
{
	class Model
	{
	public:
		Model(const std::string& filename, class Game* game);
		~Model();

		void Draw(const class Shader* shader);

	private:
		void Load(const std::string& filename);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		class Mesh* ProcessMesh(const aiMesh* mesh, const aiScene* scene);
		std::vector<class Texture*> LoadMaterialTexture(const aiMaterial* mat, const aiTextureType textureType, const aiScene* scene);
		void GenerateAABB();

	private:
		class Game* m_Game;

		std::vector<class Mesh*> m_Meshes;
		std::string m_Directory;

		AABB m_AABB;
	};
}