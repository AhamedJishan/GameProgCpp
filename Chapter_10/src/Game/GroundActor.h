#pragma once

#include "Engine/Actor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Component/BoxComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"

namespace jLab
{
	class GroundActor : public Actor
	{
	public:
		GroundActor(class Game* game)
			:Actor(game)
		{
			MeshRenderer* mr = new MeshRenderer(this);
			Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/scene/GroundPlane.obj");
			mr->SetMesh(model);
			mr->SetSpecularColor(glm::vec3(0.3f), 16);

			BoxComponent* box = new BoxComponent(this);
			box->SetObjectBox(model->GetBox());
		}
	};
}