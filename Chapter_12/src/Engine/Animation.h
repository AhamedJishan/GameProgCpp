#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "BoneTransform.h"

namespace jLab
{
	class Animation
	{
	public:
		bool Load(const std::string& filename, class Skeleton* skeleton);

		int GetNumFrames() const { return m_NumFrames; }
		float GetDuration() const { return m_Duration; }

		void GetGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, float inTime = 0.0f) const;

	private:
		BoneTransform GetBoneTransformAtFrame(int frame, const aiNodeAnim* animNode);
		glm::quat AiQuatToGlmQuat(const aiQuaternion& quat);
		glm::vec3 AiVec3ToGlmVec3(const aiVector3D& vec);

	private:
		int m_NumFrames;
		// Duration of animation in seconds
		float m_Duration;

		class Skeleton* m_Skeleton;

		std::unordered_map<std::string, int> m_BoneNameToIndexMap;
		std::vector<std::vector<BoneTransform>> m_Tracks;
	};
}