#pragma once

#include "MeshRenderer.h"
#include "Engine/MatrixPalette.h"

namespace jLab
{
	class SkinnedMeshRenderer : public MeshRenderer
	{
	public:

		SkinnedMeshRenderer(class Actor* owner, int drawOrder = 100);

		void Draw(const class Shader* shader) override;

		void SetSkeleton(class Skeleton* skeleton) { m_Skeleton = skeleton; }

		float PlayAnimation(class Animation* animation, float playRate = 1.0f);

	private:
		void ComputeMatrixPalette();

	private:
		MatrixPalette m_Palette;

		class Skeleton* m_Skeleton;
		class Animation* m_Animation;

		float m_AnimPlayRate;
		float m_AnimTime;
	};
}