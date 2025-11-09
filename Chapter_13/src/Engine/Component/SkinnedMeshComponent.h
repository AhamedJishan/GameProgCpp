#pragma once

#include "MeshComponent.h"
#include "Engine/MatrixPalette.h"

namespace jLab
{
	class SkinnedMeshComponent : public MeshComponent
	{
	public:
		SkinnedMeshComponent(class Actor* owner, int updateOrder = 100);

		void Draw(const class Shader* shader) override;
		void Update(float deltaTime) override;

		void SetSkeleton(class Skeleton* skeleton) { mSkeleton = skeleton; }

		float PlayAnimation(class Animation* animation, float playRate = 1.0f);

	private:
		void ComputeMatrixPalette();

	private:
		MatrixPalette mPalette;

		class Skeleton* mSkeleton;
		class Animation* mAnimation;

		float mAnimPlayRate;
		float mAnimTime;
	};
}