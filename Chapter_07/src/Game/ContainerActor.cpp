#include "ContainerActor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	ContainerActor::ContainerActor(Game* game)
		:Actor(game)
	{
		MeshRenderer* mc = new MeshRenderer(this);
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/container/container.obj");
		mc->SetModel(model);
		mc->SetSpecular(glm::vec3(1.0f), 16);
	}

	ContainerActor::~ContainerActor()
	{
	}

	void ContainerActor::UpdateActor(float deltaTime)
	{
	}
}