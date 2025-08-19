#include "FPSActor.h"

#include <SDL/SDL.h>
#include <glm/vec2.hpp>
#include "WallActor.h"
#include "Engine/Game.h"
#include "Engine/Collision.h"
#include "Engine/InputSystem.h"
#include "Engine/Component/BoxComponent.h"
#include "Engine/Component/MoveComponent.h"
#include "Engine/Component/FPSCameraComponent.h"
#include "Engine/Component/AudioComponent.h"
#include "Engine/AudioSystem.h"

#include "BallActor.h"

namespace jLab
{
	FPSActor::FPSActor(Game* game)
		:Actor(game)
	{
		m_MoveDir = glm::vec3(0);

		m_MoveComponent = new MoveComponent(this);
		m_FPSCameraComponent = new FPSCameraComponent(this);

		m_AudioComponent = new AudioComponent(this);
		m_Footstep = m_AudioComponent->PlayEvent("event:/Footstep");
		m_Footstep.SetPaused(true);
		m_LastFootstep = 0.0f;

		m_BoxComponent = new BoxComponent(this);
		AABB boxAABB(glm::vec3(-0.25f, 0.0f, -0.25f), glm::vec3(0.25f, 1.0f, 0.25f));
		m_BoxComponent->SetObjectBox(boxAABB);
		m_BoxComponent->SetShouldRotate(false);
	}

	void FPSActor::ActorInput(InputState& inputState)
	{
		m_MoveDir = glm::vec3(0);

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_W)) m_MoveDir.z += 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_S)) m_MoveDir.z -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_A)) m_MoveDir.x -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_D)) m_MoveDir.x += 1.0f;

		if (glm::length(m_MoveDir) > 0.1f)
			m_MoveDir = glm::normalize(m_MoveDir);


		glm::vec2 deltaMouse = inputState.Mouse.GetRelativePosition();
		const float maxMouseSpeed = 50.0f;

		float yaw = 0.0f, pitch = 0.0f;

		if (deltaMouse.x != 0.0f)
			yaw = deltaMouse.x / maxMouseSpeed * m_AngularSpeed;

		if (deltaMouse.y != 0.0f)
			pitch = deltaMouse.y / maxMouseSpeed * m_AngularSpeed;

		m_MoveComponent->SetVelocity(m_MoveDir * m_Speed);
		m_MoveComponent->SetAngularVelocity(-yaw);
		m_FPSCameraComponent->SetPitchSpeed(-pitch);

		if (inputState.Mouse.GetButtonDown(SDL_BUTTON_LEFT)) Shoot();
	}
	
	void FPSActor::ActorUpdate(float deltaTime)
	{
		FixCollisions();

		m_LastFootstep -= deltaTime;
		float velocityMagnitude = glm::length(m_MoveComponent->GetVelocity());
		if (glm::epsilonNotEqual(velocityMagnitude, 0.0f, 0.001f) && m_LastFootstep < 0.0f)
		{
			m_Footstep.SetPaused(false);
			m_Footstep.Restart();
			m_LastFootstep = 0.5f;
		}
	}

	void FPSActor::FixCollisions()
	{
		ComputeWorldTransform();

		const AABB& playerBox = m_BoxComponent->GetWorldBox();
		glm::vec3 pos = GetPosition();

		const std::vector<WallActor*>& planes = GetGame()->GetWallPlanes();

		for (WallActor* plane : planes)
		{
			plane->GetBox()->OnUpdateWorldTransform();
			const AABB& planeBox = plane->GetBox()->GetWorldBox();
			if (Intersects(playerBox, planeBox))
			{
				float dx1 = planeBox.m_Min.x - playerBox.m_Max.x;
				float dx2 = planeBox.m_Max.x - playerBox.m_Min.x;
				float dy1 = planeBox.m_Min.y - playerBox.m_Max.y;
				float dy2 = planeBox.m_Max.y - playerBox.m_Min.y;
				float dz1 = planeBox.m_Min.z - playerBox.m_Max.z;
				float dz2 = planeBox.m_Max.z - playerBox.m_Min.z;

				float dx = glm::abs(dx1) < glm::abs(dx2) ? dx1 : dx2;
				float dy = glm::abs(dy1) < glm::abs(dy2) ? dy1 : dy2;
				float dz = glm::abs(dz1) < glm::abs(dz2) ? dz1 : dz2;

				if (glm::abs(dx) <= std::abs(dy) && glm::abs(dx) <= glm::abs(dz))
					pos.x += dx;
				else if (glm::abs(dy) <= std::abs(dx) && glm::abs(dy) <= glm::abs(dz))
					pos.y += dy;
				else
					pos.z += dz;

				SetPosition(pos);
				m_BoxComponent->OnUpdateWorldTransform();
			}
		}
	}

	void FPSActor::Shoot()
	{
		m_AudioComponent->PlayEvent("event:/Shot");

		float offset = 0.025f;
		glm::vec3 screenPoint(0.0f);
		glm::vec3 start = GetGame()->GetRenderer()->ScreenToWorldPos(screenPoint);
		screenPoint.z = 0.9f;
		glm::vec3 end = GetGame()->GetRenderer()->ScreenToWorldPos(screenPoint);
		glm::vec3 dir = end - start;
		dir = glm::normalize(dir);

		BallActor* ba = new BallActor(GetGame());
		ba->SetPlayer(this);
		ba->SetPosition(start + dir * offset);
		ba->LookAt(dir);
	}
}