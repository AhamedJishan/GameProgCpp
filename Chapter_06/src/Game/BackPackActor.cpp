#include "BackPackActor.h"

#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/Game.h"

namespace jLab
{
	BackPackActor::BackPackActor(Game* game)
		:Actor(game)
	{
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/backpack/backpack.obj");
		MeshComponent* mc = new MeshComponent(this);
		mc->SetModel(model);
		mc->SetSpecular(glm::vec3(0.9), 16);
	}
	
	BackPackActor::~BackPackActor()
	{
	}
	
	void BackPackActor::UpdateActor(float deltaTime)
	{
		glm::quat rot = glm::angleAxis(glm::radians(0.4f), glm::vec3(0, 1, 0));
		SetRotation(GetRotation() * rot);
	}
}