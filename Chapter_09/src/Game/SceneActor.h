#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Components/MeshRenderer.h"

namespace jLab
{
	class SceneActor : public Actor
	{
	public:
		SceneActor(class Game* game)
			:Actor(game)
		{
			MeshRenderer* mr = new MeshRenderer(this);
			mr->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/scene/scene.obj"));
			mr->SetSpecular(glm::vec3(0.2f), 16);
		}
	};
}