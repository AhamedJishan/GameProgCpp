#include "Animation.h"

#include "Skeleton.h"

namespace jLab
{
	bool Animation::Load(const std::string& filename, Skeleton* skeleton)
	{
		m_Skeleton = skeleton;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename.c_str(), 0);

		if (!scene)
		{
			std::printf("Failed to load the animation '%s'", filename.c_str());
			return false;
		}

		if (scene->mNumAnimations <= 0)
		{
			std::printf("File '%s' doesn't have animations.", filename.c_str());
			return false;
		}

		const aiAnimation* animation = scene->mAnimations[0];
		
		float ticksPerSecond = animation->mTicksPerSecond == 0 ? 25.0f : animation->mTicksPerSecond;
		m_Duration = animation->mDuration / ticksPerSecond;
		m_NumFrames = animation->mDuration;

		std::vector<Skeleton::Bone> bones = skeleton->GetBones();
		for (int i = 0; i < skeleton->GetNumBones(); i++)
			m_BoneNameToIndexMap.emplace(bones[i].m_Name, i);


		m_Tracks.resize(m_Skeleton->GetNumBones());
		for (int i = 0; i < animation->mNumChannels; i++)
		{
			std::string nodeName = animation->mChannels[i]->mNodeName.C_Str();
			int index;
			auto it = m_BoneNameToIndexMap.find(nodeName);
			if (it != m_BoneNameToIndexMap.end())
			{
				index = m_BoneNameToIndexMap[nodeName];
			}
			else
				continue;

			m_Tracks[index].resize(m_NumFrames);
			for (int j = 0; j < m_NumFrames; j++)
			{
				BoneTransform boneTransform;
				boneTransform = GetBoneTransformAtFrame(j, animation->mChannels[i]);
				m_Tracks[index][j] = boneTransform;
			}
		}

		return true;
	}

	void Animation::GetGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, float inTime) const
	{
		if (outPoses.size() != m_Skeleton->GetNumBones())
			outPoses.resize(m_Skeleton->GetNumBones());

		// For now just calculating bone transforms at first frame of the animation
		const int frame = 0;

		glm::mat4 globalInverseTransform = m_Skeleton->GetRootNodeGlobalInverseTransform();

		// Does the root bone have a track?
		if (m_Tracks[0].size() > 0)
			outPoses[0] = globalInverseTransform * m_Tracks[0][frame].ToMatrix();
		else
			outPoses[0] = globalInverseTransform;

		for (int i = 1; i < m_Skeleton->GetNumBones(); i++)
		{
			glm::mat4 localMat = glm::mat4(1);

			if (m_Tracks[i].size() > 0)
				localMat = m_Tracks[i][frame].ToMatrix();

			outPoses[i] = outPoses[m_Skeleton->GetBones()[i].m_Parent] * localMat;
		}
	}

	BoneTransform Animation::GetBoneTransformAtFrame(int frame, const aiNodeAnim* animNode)
	{
		int posIndex = 0, nextPosIndex = 0;
		float posT = 0.0f;
		for (int i = 1; i < animNode->mNumPositionKeys - 1; i++)
		{
			if (frame < animNode->mPositionKeys[i].mTime)
			{
				posIndex = i - 1;
				nextPosIndex = i;
				posT = (frame - animNode->mPositionKeys[i - 1].mTime) / (animNode->mPositionKeys[i].mTime - animNode->mPositionKeys[i - 1].mTime);
				break;
			}
		}

		int scaleIndex = 0, nextScaleIndex = 0;
		float scaleT = 0.0f;
		for (int i = 1; i < animNode->mNumScalingKeys - 1; i++)
		{
			if (frame < animNode->mScalingKeys[i].mTime)
			{
				scaleIndex = i - 1;
				nextScaleIndex = i;
				scaleT = (frame - animNode->mScalingKeys[i - 1].mTime) / (animNode->mScalingKeys[i].mTime - animNode->mScalingKeys[i - 1].mTime);
				break;
			}
		}

		int rotIndex = 0, nextRotIndex = 0;
		float rotT = 0.0f;
		for (int i = 1; i < animNode->mNumRotationKeys- 1; i++)
		{
			if (frame < animNode->mRotationKeys[i].mTime)
			{
				rotIndex = i - 1;
				nextRotIndex = i;
				rotT = (frame - animNode->mRotationKeys[i - 1].mTime) / (animNode->mRotationKeys[i].mTime - animNode->mRotationKeys[i - 1].mTime);
				break;
			}
		}

		BoneTransform a, b;

		a.Rotation = AiQuatToGlmQuat(animNode->mRotationKeys[rotIndex].mValue);
		a.Translation = AiVec3ToGlmVec3(animNode->mPositionKeys[posIndex].mValue);
		a.Scale = AiVec3ToGlmVec3(animNode->mScalingKeys[scaleIndex].mValue);
		b.Rotation = AiQuatToGlmQuat(animNode->mRotationKeys[nextRotIndex].mValue);
		b.Translation = AiVec3ToGlmVec3(animNode->mPositionKeys[nextPosIndex].mValue);
		b.Scale = AiVec3ToGlmVec3(animNode->mScalingKeys[nextScaleIndex].mValue);

		return BoneTransform::Interpolate(a, b, posT, rotT, scaleT);
	}

	glm::quat Animation::AiQuatToGlmQuat(const aiQuaternion& quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	}

	glm::vec3 Animation::AiVec3ToGlmVec3(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}
}