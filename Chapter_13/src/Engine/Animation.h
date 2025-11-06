#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "BoneTransform.h"

namespace jLab
{
	class Animation
	{
	public:
		bool Load(const std::string& filename, class Skeleton* skeleton);

		void GetGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, float inTime = 0.0f) const;

		int GetNumFrames() const { return mNumFrames; }
		float GetDuration() const { return mDuration; }

	private:
		BoneTransform GetBoneTransformAtFrame(int frame, const aiNodeAnim* nodeAnim);
		glm::quat AiQuatToGLM(const aiQuaternion& quat);
		glm::vec3 AiVecToGLM(const aiVector3D& vec);

	private:
		int mNumFrames;
		float mDuration;

		class Skeleton* mSkeleton;

		std::vector<std::vector<BoneTransform>> mTracks;
		std::unordered_map<std::string, int> mBoneNameToIndexMap;
	};
}