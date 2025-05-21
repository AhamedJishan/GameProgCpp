#include "ProjectionArrowActor.h"

#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/InputSystem.h"
#include "Engine/Components/MeshRenderer.h"
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace jLab
{
	ProjectionArrowActor::ProjectionArrowActor(Game* game)
		:Actor(game)
	{
		m_Mesh = new MeshRenderer(this);
		m_Mesh->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/arrow/arrow.obj"));
	}

	void ProjectionArrowActor::Input(InputState& inputState)
	{
		if (!inputState.Mouse.GetButtonDown(SDL_BUTTON_LEFT))
			return;

		glm::vec3 pos, forward;
		m_Game->GetRenderer()->ScreenToWorldDir(pos, forward);

		SetPosition(pos + forward * 1.5f);
		SetRotation(glm::rotation(glm::vec3(0, 0, -1), forward));
	}

	void ProjectionArrowActor::Update(float deltaTime)
	{
	}
}