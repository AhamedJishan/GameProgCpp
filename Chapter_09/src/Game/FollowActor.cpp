#include "FollowActor.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/InputSystem.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/FollowCamera.h"
#include "Engine/Components/MoveComponent.h"

namespace jLab
{
	FollowActor::FollowActor(Game* game)
		:Actor(game)
	{
		MeshRenderer* mr = new MeshRenderer(this);
		mr->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/car/car.obj"));

		m_Camera = new FollowCamera(this);
		m_MoveComp = new MoveComponent(this);
	}
	
	void FollowActor::Input(InputState& inputState)
	{
		// WASD Movement
		m_MoveDir = glm::vec3(0, 0, 0);
		m_AngularSpeed = 0.0f;

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_W)) m_MoveDir.z += 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_S)) m_MoveDir.z -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_D)) m_AngularSpeed += m_MaxAngularSpeed;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_A)) m_AngularSpeed -= m_MaxAngularSpeed;

		if (glm::length(m_MoveDir) > 0.1f)
			m_MoveDir = glm::normalize(m_MoveDir);

		m_MoveComp->SetVelocity(m_MoveDir * m_Speed);
		m_MoveComp->SetAngularSpeed(-m_AngularSpeed);
	}
	
	void FollowActor::Update(float deltaTime)
	{
		//m_Camera->ComputeCameraPosition();
	}
}