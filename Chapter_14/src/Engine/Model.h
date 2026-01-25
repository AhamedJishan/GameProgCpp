#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Collision.h"

namespace jLab
{
	class Model
	{
	public:
		Model(const std::string& filename, class Game* game, bool flipUvs = false, class Skeleton* skeleton = nullptr);
		~Model();

		void Draw(const class Shader* shader);

		const AABB& GetBox() const { return mAABB; }
		bool IsSkinned() const { return mIsSkinned; }

	private:
		void Load(const std::string& filename);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		class Mesh* ProcessMesh(const aiMesh* mesh, const aiScene* scene);
		std::vector<class Texture*> LoadTexturesFromAiMat(const aiMaterial* material, aiTextureType textureType, const aiScene* scene);
		unsigned int GetBoneId(const aiBone* bone);

		void GenerateAABB();

	private:
		class Game* mGame;
		class Skeleton* mSkeleton;
		bool mIsSkinned;
		bool mFlipUVs;

		std::string mDirectory;
		std::vector<class Mesh*> mMeshes;
		std::unordered_map<std::string, uint8_t> mBoneNameToIndexMap;

		AABB mAABB;
	};
}