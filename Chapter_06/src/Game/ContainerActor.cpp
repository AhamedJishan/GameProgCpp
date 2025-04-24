#include "ContainerActor.h"

#include "Engine/Component/MeshComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Texture.h"
#include "Engine/Model.h"

namespace jLab
{
	ContainerActor::ContainerActor(Game* game)
		:Actor(game)
	{
		MeshComponent* mc = new MeshComponent(this);
		mc->SetModel(m_Game->GetRenderer()->GetModel("Assets/Models/container/container.obj"));
		mc->SetSpecular(glm::vec3(0.9), 32);
	}

	ContainerActor::~ContainerActor()
	{
	}

	void ContainerActor::UpdateActor(float deltaTime)
	{
	}
}