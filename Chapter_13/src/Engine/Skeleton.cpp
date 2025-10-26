#include "Skeleton.h"

#include <cstdio>
#include <assimp/Importer.hpp>

namespace jLab
{
	Skeleton::Skeleton()
	{
	}

	bool Skeleton::Load(const std::string& filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, 0);

		if (!scene)
		{
			printf("ERROR: Failed to load skeleton '%s'", filename.c_str());
			return false;
		}

		mRootNodeGlobalInverseBindPose = glm::inverse(AiMatToGLM(scene->mRootNode->mTransformation));

		std::unordered_map<std::string, glm::mat4> boneNameToOffsetMap;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			for (int j = 0; j < mesh->mNumBones; j++)
			{
				const aiBone* bone = mesh->mBones[j];
				std::string name = bone->mName.C_Str();
				
				boneNameToOffsetMap[name] = AiMatToGLM(bone->mOffsetMatrix);
			}
		}

		ProcessNode(scene->mRootNode, -1, boneNameToOffsetMap);

		return true;
	}

	void Skeleton::ProcessNode(const aiNode* node, int parentIndex, std::unordered_map<std::string, glm::mat4>& boneNameToOffsetMap)
	{
		int thisIndex = -1;

		std::string name = node->mName.C_Str();
		auto it = boneNameToOffsetMap.find(name);
		if (it != boneNameToOffsetMap.end())
		{
			Bone bone;
			bone.Name = name;
			bone.Parent = parentIndex;
			bone.LocalBindPose = AiMatToBoneTransform(node->mTransformation);

			thisIndex = mBones.size();

			mBones.emplace_back(bone);
			mGlobalInverseBindPoses.emplace_back(boneNameToOffsetMap[name]);
		}

		for (int i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], (thisIndex == -1 ? parentIndex : thisIndex), boneNameToOffsetMap);
	}

	glm::mat4 Skeleton::AiMatToGLM(const aiMatrix4x4& m)
	{
		glm::mat4 r;
		r[0][0] = m.a1; r[1][0] = m.a2; r[2][0] = m.a3; r[3][0] = m.a4;
		r[0][1] = m.b1; r[1][1] = m.b2; r[2][1] = m.b3; r[3][1] = m.b4;
		r[0][2] = m.c1; r[1][2] = m.c2; r[2][2] = m.c3; r[3][2] = m.c4;
		r[0][3] = m.d1; r[1][3] = m.d2; r[2][3] = m.d3; r[3][3] = m.d4;
		return r;
	}

	glm::mat4 Skeleton::AiMatToGLM(const aiMatrix3x3& m)
	{
		glm::mat4 r(1.0f);
		r[0][0] = m.a1; r[1][0] = m.a2; r[2][0] = m.a3;
		r[0][1] = m.b1; r[1][1] = m.b2; r[2][1] = m.b3;
		r[0][2] = m.c1; r[1][2] = m.c2; r[2][2] = m.c3;

		return r;
	}

	BoneTransform Skeleton::AiMatToBoneTransform(const aiMatrix4x4& aiMat)
	{
		aiVector3D position, scale;
		aiQuaternion rotation;

		aiMat.Decompose(scale, rotation, position);

		BoneTransform boneTransform;
		boneTransform.Rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		boneTransform.Position = glm::vec3(position.x, position.y, position.z);
		boneTransform.Scale = glm::vec3(scale.x, scale.y, scale.z);

		return boneTransform;
	}
}