#include "SceneActor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Model.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	SceneActor::SceneActor(Game* game)
		:Actor(game)
	{
		MeshRenderer* mc = new MeshRenderer(this);
		Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/scene/scene.obj");
		mc->SetModel(model);
		mc->SetSpecular(glm::vec3(0.2f), 16);
	}

	SceneActor::~SceneActor()
	{
	}

	void SceneActor::UpdateActor(float deltaTime)
	{
	}
}