#include "BagActor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	BagActor::BagActor(Game* game)
		:Actor(game)
	{
		MeshRenderer* mc = new MeshRenderer(this);
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/backpack/backpack.obj");
		mc->SetModel(model);
		mc->SetSpecular(glm::vec3(1.0f), 32);
	}

	BagActor::~BagActor()
	{
	}

	void BagActor::UpdateActor(float deltaTime)
	{
		Rotate(glm::radians(0.5f), glm::vec3(0, 1, 0));
	}
}