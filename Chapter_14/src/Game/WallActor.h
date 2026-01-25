#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Component/MeshComponent.h"
#include "Engine/Component/BoxComponent.h"

namespace jLab
{
	class WallActor : public Actor
	{
	public:
		WallActor(class Game* game)
			:Actor(game)
		{
			MeshComponent* mr = new MeshComponent(this);
			Model* model = GetGame()->GetRenderer()->GetModel("Assets/Models/scene/Wall.obj");
			mr->SetMesh(model);
			mr->SetSpecular(glm::vec3(0.3f), 16.0f);

			mBox = new BoxComponent(this);
			mBox->SetObjectBox(model->GetBox());
		}

		BoxComponent* GetBox() const { return mBox; }

	private:
		BoxComponent* mBox;
	};
}