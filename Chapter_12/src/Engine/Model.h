#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Collision.h"

namespace jLab
{
	class Model
	{
	public:
		Model(const std::string& filename, class Game* game, bool flipUVs = true, class Skeleton* = nullptr);
		~Model();

		void Draw(const class Shader* shader);

		const AABB& GetBox() { return m_AABB; }
		bool IsSkinned() const { return m_IsSkinned; }

	private:
		void Load(const std::string& filename);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		class Mesh* ProcessMesh(const aiMesh* mesh, const aiScene* scene);
		std::vector<class Texture*> LoadMaterialTexture(const aiMaterial* mat, const aiTextureType textureType, const aiScene* scene);

		unsigned int GetBoneId(const aiBone* bone);

		void GenerateAABB();

	private:
		class Game* m_Game;
		class Skeleton* m_Skeleton;
		bool m_IsSkinned;
		bool m_FlipUVs;

		std::vector<class Mesh*> m_Meshes;
		std::string m_Directory;

		std::unordered_map<std::string, uint8_t> m_BoneNameToIndexMapping;
		//std::unordered_map<std::string, class Texture*> m_EmbeddedTextures;

		AABB m_AABB;
	};
}