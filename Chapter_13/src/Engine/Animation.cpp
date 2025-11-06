#include "Animation.h"

#include <cstdio>
#include "Skeleton.h"

namespace jLab
{
	bool Animation::Load(const std::string& filename, Skeleton* skeleton)
	{
		mSkeleton = skeleton;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, 0);
		if (!scene)
		{
			printf("Failed to load animation '%s'\n", filename.c_str());
			return false;
		}
		if (scene->mNumAnimations <= 0)
		{
			printf("File '%s' doesn't have animations.\n", filename.c_str());
			return false;
		}

		const aiAnimation* animation = scene->mAnimations[0];

		float ticksPerSecond = animation->mTicksPerSecond == 0 ? 25.0f : animation->mTicksPerSecond;
		mNumFrames = animation->mDuration;
		mDuration = mNumFrames / ticksPerSecond;

		std::vector<Skeleton::Bone> bones = mSkeleton->GetBones();
		for (int i = 0; i < bones.size(); i++)
			mBoneNameToIndexMap.emplace(bones[i].Name, i);

		mTracks.resize(bones.size());
		for (int i = 0; i < animation->mNumChannels; i++)
		{
			std::string boneName = animation->mChannels[i]->mNodeName.C_Str();
			int index;
			auto it = mBoneNameToIndexMap.find(boneName);
			if (it != mBoneNameToIndexMap.end())
				index = it->second;
			else
				continue;

			mTracks[index].resize(mNumFrames);
			for (int j = 0; j < mNumFrames; i++)
			{
				BoneTransform boneTransform;
				boneTransform = GetBoneTransformAtFrame(j, animation->mChannels[index]);
				mTracks[index][j] = boneTransform;
			}
		}

		return true;
	}

	void Animation::GetGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, float inTime) const
	{
		if(outPoses.size() != mSkeleton->GetNumBones())
			outPoses.resize(mSkeleton->GetNumBones());

		int frame = (int)(inTime / mDuration * mNumFrames);

		glm::mat4 rootNodeGlobalInverseTransform = mSkeleton->GetRootNodeGlobalInverseBindPose();

		if (frame > mNumFrames - 1)
			frame = mNumFrames - 1;

		if (mTracks[0].size() > 0)
			outPoses[0] = rootNodeGlobalInverseTransform * mTracks[0][frame].ToMatrix();
		else
			outPoses[0] = rootNodeGlobalInverseTransform;

		for (int i = 1; i < mSkeleton->GetNumBones(); i++)
		{
			glm::mat4 localMat = glm::mat4(1);

			if (mTracks[i].size() > 0)
				localMat = mTracks[i][frame].ToMatrix();

			outPoses[i] = outPoses[mSkeleton->GetBones()[i].Parent] * localMat;
		}
	}
	
	BoneTransform Animation::GetBoneTransformAtFrame(int frame, const aiNodeAnim* nodeAnim)
	{
		int posIndex = 0, nextPosIndex = 0;
		float posT = 0.0f;
		for (int i = 1; i < nodeAnim->mNumPositionKeys; i++)
		{
			if (frame < nodeAnim->mPositionKeys[i].mTime)
			{
				posIndex = i - 1;
				nextPosIndex = i;
				posT = (frame - nodeAnim->mPositionKeys[posIndex].mTime) / 
					(nodeAnim->mPositionKeys[nextPosIndex].mTime - nodeAnim->mPositionKeys[posIndex].mTime);
				break;
			}
		}

		int rotIndex = 0, nextRotIndex = 0;
		float rotT = 0.0f;
		for (int i = 1; i < nodeAnim->mNumRotationKeys; i++)
		{
			if (frame < nodeAnim->mRotationKeys[i].mTime)
			{
				rotIndex = i - 1;
				nextRotIndex = i;
				rotT = (frame - nodeAnim->mRotationKeys[rotIndex].mTime) /
					(nodeAnim->mRotationKeys[nextRotIndex].mTime - nodeAnim->mRotationKeys[rotIndex].mTime);
				break;
			}
		}

		int scaleIndex = 0, nextScaleIndex = 0;
		float scaleT = 0.0f;
		for (int i = 1; i < nodeAnim->mNumScalingKeys; i++)
		{
			if (frame < nodeAnim->mScalingKeys[i].mTime)
			{
				scaleIndex = i - 1;
				nextScaleIndex = i;
				scaleT = (frame - nodeAnim->mScalingKeys[scaleIndex].mTime) /
					(nodeAnim->mScalingKeys[nextScaleIndex].mTime - nodeAnim->mScalingKeys[scaleIndex].mTime);
				break;
			}
		}

		BoneTransform a, b;
		a.Position = AiVecToGLM(nodeAnim->mPositionKeys[posIndex].mValue);
		b.Position = AiVecToGLM(nodeAnim->mPositionKeys[nextPosIndex].mValue);
		a.Rotation = AiQuatToGLM(nodeAnim->mRotationKeys[rotIndex].mValue);
		b.Rotation = AiQuatToGLM(nodeAnim->mRotationKeys[nextRotIndex].mValue);
		a.Scale = AiVecToGLM(nodeAnim->mScalingKeys[scaleIndex].mValue);
		b.Scale = AiVecToGLM(nodeAnim->mScalingKeys[nextScaleIndex].mValue);

		return BoneTransform::Interpolate(a, b, posT, rotT, scaleT);
	}
	
	glm::quat Animation::AiQuatToGLM(const aiQuaternion& quat)
	{
		return glm::quat(quat.w, quat.x, quat.y, quat.z);
	}
	
	glm::vec3 Animation::AiVecToGLM(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}
}