#pragma once

#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Actor.h"
#include "Engine/Component/BoxComponent.h"
#include "Engine/Component/MeshRenderer.h"
#include "Engine/Model.h"

namespace jLab
{
	class TargetActor : public Actor
	{
	public:
		TargetActor(Game* game)
			:Actor(game)
		{
			SetScale(glm::vec3(0.75f));
			Rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));

			Model* model = GetGame()->GetRenderer()->GetModel("Assets/Models/target/target.obj");
			MeshRenderer* mr = new MeshRenderer(this);
			mr->SetMesh(model);
			mr->SetSpecularColor(glm::vec3(0.0f), 32.0f);

			BoxComponent* bc = new BoxComponent(this);
			bc->SetObjectBox(model->GetBox());
		}
	};
}