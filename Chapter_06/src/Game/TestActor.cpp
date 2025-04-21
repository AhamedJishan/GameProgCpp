#include "TestActor.h"

#include "Engine/Model.h"
#include "Engine/Component/MeshComponent.h"

namespace jLab
{
	TestActor::TestActor(Game* game)
		:Actor(game)
	{
		Model* model = new Model("Assets/Models/backpack/backpack.obj", m_Game);
		MeshComponent* mc = new MeshComponent(this);
		mc->SetModel(model);
	}
	
	TestActor::~TestActor()
	{
	}
	
	void TestActor::UpdateActor(float deltaTime)
	{
		glm::quat rot = glm::angleAxis(glm::radians(0.25f), glm::vec3(0, 1, 0));
		SetRotation(rot * GetRotation());
	}
}