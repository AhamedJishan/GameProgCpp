#include "SkinnedMeshComponent.h"

#include <vector>
#include <cstdio>
#include <glm/mat4x4.hpp>
#include "Engine/Skeleton.h"
#include "Engine/Animation.h"
#include "Engine/Shader.h"

namespace jLab
{
	SkinnedMeshComponent::SkinnedMeshComponent(Actor* owner, int updateOrder)
		:MeshComponent(owner, true, updateOrder)
		,mSkeleton(nullptr)
		,mAnimation(nullptr)
	{
	}

	void SkinnedMeshComponent::Draw(const Shader* shader)
	{
		shader->SetMat4s("uMatrixPalette", mPalette.mEntry, MAX_NUM_BONES);
		MeshComponent::Draw(shader);
	}
	
	void SkinnedMeshComponent::Update(float deltaTime)
	{
		if (mSkeleton && mAnimation)
		{
			mAnimTime += deltaTime * mAnimPlayRate;
			while (mAnimTime >= mAnimation->GetDuration())
				mAnimTime -= mAnimation->GetDuration();

			ComputeMatrixPalette();
		}
	}
	
	float SkinnedMeshComponent::PlayAnimation(Animation* animation, float playRate)
	{
		mAnimation = animation;
		mAnimPlayRate = playRate;
		mAnimTime = 0.0f;

		if (!mAnimation)
			return 0.0f;

		ComputeMatrixPalette();
		return mAnimation->GetDuration();
	}

	void SkinnedMeshComponent::ComputeMatrixPalette()
	{
		if (!mSkeleton)
		{
			printf("No Skeleton attached!");
			return;
		}

		std::vector<glm::mat4> globalInversePoses = mSkeleton->GetGlobalInverseBindPoses();
		std::vector<glm::mat4> currentPose;
		mAnimation->GetGlobalPoseAtTime(currentPose, mAnimTime);

		for (int i = 0; i < mSkeleton->GetNumBones(); i++)
			mPalette.mEntry[i] = currentPose[i] * globalInversePoses[i];
	}
}