#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/Component/BoxComponent.h"

namespace jLab
{
	class GroundActor : public Actor
	{
	public:
		GroundActor(class Game* game)
			:Actor(game)
		{
			MeshComponent* mc = new MeshComponent(this);
			Model* model = mGame->GetRenderer()->GetModel("Assets/Models/scene/GroundPlane.obj");
			mc->SetMesh(model);
			mc->SetSpecular(glm::vec3(0.3f), 16);

			BoxComponent* box = new BoxComponent(this);
			box->SetObjectBox(model->GetBox());
		}
	};
}