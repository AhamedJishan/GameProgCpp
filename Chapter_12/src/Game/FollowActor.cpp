#include "FollowActor.h"

#include <Engine/Game.h>
#include <Engine/Renderer.h>
#include <Engine/Model.h>
#include <Engine/Skeleton.h>
#include <Engine/Animation.h>
#include <Engine/Component/SkinnedMeshRenderer.h>
#include <Engine/Component/FollowCamera.h>

namespace jLab
{
	FollowActor::FollowActor(Game* game)
		:Actor(game)
	{
		Skeleton* skeleton = m_Game->GetSkeleton("Assets/Models/eve/eve.dae");
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/eve/eve.dae", false, skeleton);

		m_IdleAnim = m_Game->GetAnimation("Assets/Models/eve/eve_idle.dae", skeleton);
		m_RunAnim = m_Game->GetAnimation("Assets/Models/eve/eve_run.dae", skeleton);

		m_SkinnedMeshRenderer = new SkinnedMeshRenderer(this);
		m_SkinnedMeshRenderer->SetMesh(model);
		m_SkinnedMeshRenderer->SetSkeleton(skeleton);
		m_SkinnedMeshRenderer->SetSpecularColor(glm::vec3(0.3f), 16);
		m_SkinnedMeshRenderer->PlayAnimation(m_IdleAnim);

		SetScale(glm::vec3(0.00006f));
		SetPosition(glm::vec3(0, 0.25f, 0));
	}

	void FollowActor::ActorInput(InputState& inputState)
	{
	}
}