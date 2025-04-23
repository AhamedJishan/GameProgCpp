#include "TestActor.h"

#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/Game.h"

namespace jLab
{
	TestActor::TestActor(Game* game)
		:Actor(game)
	{
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/backpack/backpack.obj");
		MeshComponent* mc = new MeshComponent(this);
		mc->SetModel(model);
	}
	
	TestActor::~TestActor()
	{
	}
	
	void TestActor::UpdateActor(float deltaTime)
	{
		glm::quat rot = glm::angleAxis(glm::radians(0.05f), glm::vec3(0, 1, 0));
		SetRotation(GetRotation() * rot);
	}
}