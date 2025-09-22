#include "SkinnedMeshRenderer.h"

#include <vector>
#include <glm/mat4x4.hpp>

#include "Engine/Skeleton.h"
#include "Engine/Animation.h"
#include "Engine/Shader.h"


namespace jLab
{
	SkinnedMeshRenderer::SkinnedMeshRenderer(Actor* owner, int drawOrder)
		:MeshRenderer(owner, true, drawOrder)
	{
	}

	void SkinnedMeshRenderer::Draw(const Shader* shader)
	{
		shader->SetMat4s("u_MatrixPalette", m_Palette.m_Entry, MAX_SKELETON_BONES);
		MeshRenderer::Draw(shader);
	}

	float SkinnedMeshRenderer::PlayAnimation(Animation* animation, float playRate)
	{
		m_Animation = animation;
		m_AnimPlayRate = playRate;
		m_AnimTime = 0.0f;

		if (!m_Animation)
			return 0.0f;

		ComputeMatrixPalette();

		return m_Animation->GetDuration();
	}

	void SkinnedMeshRenderer::ComputeMatrixPalette()
	{
		if (!m_Skeleton)
		{
			std::printf("No skeleton attached!");
			return;
		}

		std::vector<glm::mat4> globalInvBindPoses = m_Skeleton->GetGlobalInverseBindPoses();
		std::vector<glm::mat4> currentPoses;
		m_Animation->GetGlobalPoseAtTime(currentPoses, 0.0f);

		for (int i = 0; i < m_Skeleton->GetNumBones(); i++)
			m_Palette.m_Entry[i] = currentPoses[i] * globalInvBindPoses[i];
	}
}