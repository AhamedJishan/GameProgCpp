#include "OrbitActor.h"

#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/InputSystem.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/OrbitCamera.h"

namespace jLab
{
	OrbitActor::OrbitActor(Game* game)
		:Actor(game)
	{
		m_Camera = new OrbitCamera(this);
		m_PitchInput = 0.0f;
		m_YawInput = 0.0f;

		m_Mesh = new MeshRenderer(this);
		m_Mesh->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/car/car.obj"));
	}
	
	void OrbitActor::Input(InputState& inputState)
	{
		m_PitchInput = 0.0f;
		m_YawInput = 0.0f;

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_W)) m_PitchInput -= m_MaxPitchInput;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_S)) m_PitchInput += m_MaxPitchInput;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_A)) m_YawInput -= m_MaxYawInput;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_D)) m_YawInput += m_MaxYawInput;
	}
	
	void OrbitActor::Update(float deltaTime)
	{
		m_Camera->AddYawSpeed(m_YawInput * deltaTime);
		m_Camera->AddPitchSpeed(m_PitchInput * deltaTime);
	}

	void OrbitActor::SetVisible(bool visible)
	{
		m_Mesh->SetVisible(visible);
	}
}