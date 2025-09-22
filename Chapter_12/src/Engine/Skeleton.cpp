#include "Skeleton.h"

namespace jLab
{
	Skeleton::Skeleton()
	{
	}

	bool Skeleton::Load(const std::string& filename)
	{
		std::unordered_map<std::string, glm::mat4> boneNamesToOffsetMap;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename.c_str(), 0);
		if (!scene)
		{
			std::printf("Failed to load the skeleton '%s'", filename.c_str());
			return false;
		}

		m_RootNodeGlobalInverseTransform = glm::inverse(AiMatToGLM(scene->mRootNode->mTransformation));

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			for (int j = 0; j < mesh->mNumBones; j++)
			{
				aiBone* bone = mesh->mBones[j];
				std::string name = bone->mName.C_Str();
				glm::mat4 offsetMat = AiMatToGLM(bone->mOffsetMatrix);

				boneNamesToOffsetMap[name] = offsetMat;
			}
		}

		ProcessNode(scene->mRootNode, -1, boneNamesToOffsetMap);
		return true;
	}

	void Skeleton::ProcessNode(const aiNode* node, int parentIndex, std::unordered_map<std::string, glm::mat4>& boneNamesToOffsetMap)
	{
		std::string nodeName = node->mName.C_Str();
		auto it = boneNamesToOffsetMap.find(nodeName);

		int thisIndex = -1;
		if (it != boneNamesToOffsetMap.end())
		{
			BoneTransform boneTransform;

			Bone bone;
			bone.m_Name = nodeName;
			bone.m_Parent = parentIndex;
			bone.m_LocalBindPose = AiMatToBoneTransform(node->mTransformation);

			thisIndex = m_Bones.size();
			m_Bones.emplace_back(bone);
			m_GlobalInverseBindPose.emplace_back(boneNamesToOffsetMap[nodeName]);
		}

		for (int i = 0; i < node->mNumChildren; i++)
			ProcessNode(node->mChildren[i], (thisIndex == -1 ? parentIndex : thisIndex), boneNamesToOffsetMap);
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
		boneTransform.Translation = glm::vec3(position.x, position.y, position.z);
		boneTransform.Scale = glm::vec3(scale.x, scale.y, scale.z);

		return boneTransform;
	}
}