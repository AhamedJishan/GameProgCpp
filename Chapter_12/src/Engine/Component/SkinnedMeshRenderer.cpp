#include "SkinnedMeshRenderer.h"

namespace jLab
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(Actor* owner, int drawOrder)
		:MeshRenderer(owner, drawOrder)
	{
	}

	void SkinnedMeshRenderer::Draw(const Shader* shader)
	{
		MeshRenderer::Draw(shader);
	}
}