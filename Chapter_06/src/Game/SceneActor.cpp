#include "SceneActor.h"

#include "Engine/Component/MeshComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"

namespace jLab
{
	SceneActor::SceneActor(Game* game)
		:Actor(game)
	{
		MeshComponent* mc = new MeshComponent(this);
		mc->SetModel(m_Game->GetRenderer()->GetModel("Assets/Models/scene/scene.obj"));
		mc->SetSpecular(glm::vec3(0.2f), 8);
	}

	SceneActor::~SceneActor()
	{
	}

	void SceneActor::InputActor(const uint8_t* keyState)
	{
	}

	void SceneActor::UpdateActor(float deltaTime)
	{
	}
}