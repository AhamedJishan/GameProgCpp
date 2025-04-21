#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace jLab
{
	class Model
	{
	public:
		Model(const std::string& path, class Game* game);
		~Model();

		void Draw(const class Shader* shader);

	private:
		void Load(const std::string& path);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		class Mesh* ProcessMesh(const aiMesh* mesh, const aiScene* scene);
		std::vector<class Texture*> LoadMaterialTextures(const aiMaterial* mat, aiTextureType textureType, const aiScene* scene);

	private:
		std::vector<class Mesh*> m_Meshes;
		std::string m_Directory;

		class Game* m_Game;
	};
}