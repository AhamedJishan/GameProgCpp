#include "FPSActor.h"

#include "Engine/Game.h"
#include "Engine/InputSystem.h"
#include "Engine/Components/FPSCamera.h"

namespace jLab
{
	FPSActor::FPSActor(Game* game)
		:Actor(game)
	{
		m_MoveDir = glm::vec3(0);
		m_Camera = new FPSCamera(this);
	}
	
	void FPSActor::Input(InputState& inputState)
	{
		// WASD Movement
		m_MoveDir = glm::vec3(0, 0, 0);

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_W)) m_MoveDir.z += 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_S)) m_MoveDir.z -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_D)) m_MoveDir.x += 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_A)) m_MoveDir.x -= 1.0f;

		if (glm::length(m_MoveDir) > 0.1f)
			m_MoveDir = glm::normalize(m_MoveDir);

		// Horizontal Mouse Movement
		glm::vec2 deltaMouse = inputState.Mouse.GetPosition();
		const float maxMouseSpeed = 500.0f;

		m_AngularSpeed = 0.0f;
		if (deltaMouse.x != 0)
			m_AngularSpeed = (deltaMouse.x / maxMouseSpeed) * m_MaxAngularSpeed;

		// Vertical Mouse Movement
		float pitchSpeed = 0.0f;
		if (deltaMouse.y != 0)
			pitchSpeed = (deltaMouse.y / maxMouseSpeed) * m_MaxPitchSpeed;
		m_Camera->SetPitchSpeed(-pitchSpeed);
	}
	
	void FPSActor::Update(float deltaTime)
	{
		if (glm::length(m_MoveDir) > 0.1f)
		{
			glm::vec3 worldMoveDir = GetForward() * m_MoveDir.z + GetRight() * m_MoveDir.x;
			SetPosition(GetPosition() + (worldMoveDir * m_Speed * deltaTime));
		}

		if (m_AngularSpeed != 0.0f)
			Rotate(-m_AngularSpeed, glm::vec3(0, 1, 0));
	}
}