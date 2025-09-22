#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "BoneTransform.h"

namespace jLab
{
	class Skeleton
	{
	public:
		struct Bone
		{
			BoneTransform m_LocalBindPose;
			std::string m_Name;
			int m_Parent;
		};

		Skeleton();

		bool Load(const std::string& filename);

		int GetNumBones() const { return m_Bones.size(); }
		const Bone& GetBone(int id) const { return m_Bones[id]; }
		const std::vector<Bone>& GetBones() const { return m_Bones; }
		const std::vector<glm::mat4>& GetGlobalInverseBindPoses() const { return m_GlobalInverseBindPose; }
		const glm::mat4 GetRootNodeGlobalInverseTransform() const { return m_RootNodeGlobalInverseTransform; }

	private:
		void ProcessNode(const aiNode* node, int parentIndex, std::unordered_map<std::string, glm::mat4>& boneNamesToOffsetMap);

		glm::mat4 AiMatToGLM(const aiMatrix4x4& m);
		glm::mat4 AiMatToGLM(const aiMatrix3x3& m);
		BoneTransform AiMatToBoneTransform(const aiMatrix4x4& aiMat);

	private:
		std::vector<Bone> m_Bones;
		std::vector<glm::mat4> m_GlobalInverseBindPose;
		glm::mat4 m_RootNodeGlobalInverseTransform;
	};
}