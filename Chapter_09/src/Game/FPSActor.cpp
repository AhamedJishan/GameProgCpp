#include "FPSActor.h"

#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/InputSystem.h"
#include "Engine/Components/FPSCamera.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/MoveComponent.h"

namespace jLab
{
	FPSActor::FPSActor(Game* game)
		:Actor(game)
	{
		m_MoveComp = new MoveComponent(this);
		m_MoveDir = glm::vec3(0);
		m_Camera = new FPSCamera(this);

		m_FpsModel = new Actor(m_Game);
		MeshRenderer* mr = new MeshRenderer(m_FpsModel);
		mr->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/gun/gun.obj"));
		mr->SetSpecular(glm::vec3(0.5f), 32.0f);
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

		m_MoveComp->SetVelocity(m_MoveDir * m_Speed);
		m_MoveComp->SetAngularSpeed(-m_AngularSpeed);
	}
	
	void FPSActor::Update(float deltaTime)
	{
		// Making the fps Model follow fpsactor
		const glm::vec3 modelOffset = glm::vec3(0.1, -0.1, 0.1);
		glm::vec3 modelPos = GetPosition();
		modelPos += GetForward() * modelOffset.z;
		modelPos += GetRight() * modelOffset.x;
		modelPos += GetUp() * modelOffset.y;
		m_FpsModel->SetPosition(modelPos);

		glm::quat actorRot = GetRotation();
		glm::quat cameraRot = glm::angleAxis(m_Camera->GetPitch(), GetRight());
		m_FpsModel->SetRotation(cameraRot * actorRot);
	}
}