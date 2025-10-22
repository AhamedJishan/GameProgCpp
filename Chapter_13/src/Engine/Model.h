#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "Collision.h"

namespace jLab
{
	class Model
	{
	public:
		Model(const std::string& filename, class Game* game, bool flipUvs = false, class Skeleton* skeleton = nullptr);
		~Model();

	private:
		void Load();

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