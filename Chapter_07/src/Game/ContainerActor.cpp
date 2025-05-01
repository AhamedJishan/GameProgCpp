#include "ContainerActor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Component/AudioComponent.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"


namespace jLab
{
	ContainerActor::ContainerActor(Game* game)
		:Actor(game),
		m_SpaceKeyDown(false)
	{
		MeshRenderer* mc = new MeshRenderer(this);
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/container/container.obj");
		mc->SetModel(model);
		mc->SetSpecular(glm::vec3(1.0f), 16);

		m_explosionAudio = new AudioComponent(this);
	}

	ContainerActor::~ContainerActor()
	{
	}

	void ContainerActor::InputActor(const uint8_t* keyState)
	{
		if (keyState[SDL_SCANCODE_SPACE] && !m_SpaceKeyDown)
		{
			m_explosionAudio->PlayEvent("event:/Explosion2D");
			m_SpaceKeyDown = true;
		}
		else if (!keyState[SDL_SCANCODE_SPACE] && m_SpaceKeyDown)
			m_SpaceKeyDown = false;
	}

	void ContainerActor::UpdateActor(float deltaTime)
	{
	}
}