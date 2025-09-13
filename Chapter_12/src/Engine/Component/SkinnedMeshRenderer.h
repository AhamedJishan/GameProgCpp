#pragma once

#include "MeshRenderer.h"

namespace jLab
{
	class SkinnedMeshRenderer : public MeshRenderer
	{
	public:
		SkinnedMeshRenderer(class Actor* owner, int drawOrder = 100);

		void Draw(const class Shader* shader) override;

		void SetSkeleton(class Skeleton* skeleton) { m_Skeleton = skeleton; }

	private:
		class Skeleton* m_Skeleton;
	};
}