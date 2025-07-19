#include "FPSActor.h"

#include "Engine/Component/MoveComponent.h"
#include "Engine/Component/FPSCameraComponent.h"
#include "Engine/InputSystem.h"
#include <SDL/SDL.h>
#include <glm/vec2.hpp>

namespace jLab
{
	FPSActor::FPSActor(Game* game)
		:Actor(game)
	{
		m_MoveDir = glm::vec3(0);

		m_MoveComponent = new MoveComponent(this);
		m_FPSCameraComponent = new FPSCameraComponent(this);
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


		glm::vec2 deltaMouse = inputState.Mouse.GetPosition();
		const float maxMouseSpeed = 500.0f;

		float yaw = 0.0f, pitch = 0.0f;

		if (deltaMouse.x != 0.0f)
			yaw = deltaMouse.x / maxMouseSpeed * m_AngularSpeed;

		if (deltaMouse.y != 0.0f)
			pitch = deltaMouse.y / maxMouseSpeed * m_AngularSpeed;

		m_MoveComponent->SetVelocity(m_MoveDir * m_Speed);
		m_MoveComponent->SetAngularVelocity(-yaw);
		m_FPSCameraComponent->SetPitchSpeed(-pitch);
	}
	
	void FPSActor::ActorUpdate(float deltaTime)
	{
	}
}