#include "FollowActor.h"

#include <Engine/Game.h>
#include <Engine/Renderer.h>
#include <Engine/Model.h>
#include <Engine/Skeleton.h>
#include <Engine/Animation.h>
#include <Engine/InputSystem.h>
#include <Engine/Component/SkinnedMeshRenderer.h>
#include <Engine/Component/FollowCamera.h>
#include <Engine/Component/MoveComponent.h>

namespace jLab
{
	FollowActor::FollowActor(Game* game)
		:Actor(game)
		,m_IsMoving(false)
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

		m_MoveComp = new MoveComponent(this);
		m_FollowCam = new FollowCamera(this);

		m_FollowCam->SetOffset(glm::vec3(0.0f, 0.775f, 0.5f));
		m_FollowCam->SetTargetDistance(-5.0f);
		m_FollowCam->SetSpringConstant(100.0f);
		m_FollowCam->SnapToIdeal();

		SetScale(glm::vec3(0.00006f));
		SetPosition(glm::vec3(0, 0.25f, 0));
	}

	void FollowActor::ActorInput(InputState& inputState)
	{
		// WASD Movement
		m_MoveDir = glm::vec3(0, 0, 0);
		m_AngularSpeed = 0.0f;

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_UP)) m_MoveDir.z -= 1.0f;
		//if (inputState.Keyboard.GetKey(SDL_SCANCODE_DOWN)) m_MoveDir.z += 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_RIGHT)) m_AngularSpeed += m_MaxAngularSpeed;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_LEFT)) m_AngularSpeed -= m_MaxAngularSpeed;

		if (glm::length(m_MoveDir) > 0.1f)
			m_MoveDir = glm::normalize(m_MoveDir);

		if (!m_IsMoving && glm::length(m_MoveDir) > 0.1f)
		{
			m_IsMoving = true;
			m_SkinnedMeshRenderer->PlayAnimation(m_RunAnim);
		}

		if (m_IsMoving && glm::length(m_MoveDir) < 0.1f)
		{
			m_IsMoving = false;
			m_SkinnedMeshRenderer->PlayAnimation(m_IdleAnim);
		}

		m_MoveComp->SetVelocity(m_MoveDir * m_Speed);
		m_MoveComp->SetAngularVelocity(-m_AngularSpeed);
	}
}