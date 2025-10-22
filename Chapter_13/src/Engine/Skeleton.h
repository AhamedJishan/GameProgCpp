#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <assimp/scene.h>

#include "BoneTransform.h"

namespace jLab
{
	class Skeleton
	{
	public:
		struct Bone
		{
			int Parent;
			std::string Name;
			BoneTransform LocalBindPose;
		};

	public:
		Skeleton();

		bool Load(const std::string& filename);

		const Bone& GetBone(int id) const { return mBones[id]; }
		const std::vector<Bone>& GetBones() const { return mBones; }
		const std::vector<glm::mat4>& GetGlobalInverseBindPoses() const { return mGlobalInverseBindPoses; }
		const glm::mat4& GetRootNodeGlobalInverseBindPose() const { return mRootNodeGlobalInverseBindPose; }

	private:
		void ProcessNode(const aiNode* node, int parentIndex, const std::unordered_map<std::string, glm::mat4>& boneNameToOffsetMap);

		glm::mat4 AiMatToGLM(const aiMatrix4x4& m);
		glm::mat4 AiMatToGLM(const aiMatrix3x3& m);
		BoneTransform AiMatToBoneTransform(const aiMatrix4x4& aiMat);

	private:
		std::vector<Bone> mBones;
		std::vector<glm::mat4> mGlobalInverseBindPoses;
		glm::mat4 mRootNodeGlobalInverseBindPose;
	};
}