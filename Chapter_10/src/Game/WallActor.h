#pragma once

#include "Engine/Actor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Component/BoxComponent.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"

namespace jLab
{
	class WallActor : public Actor
	{
	public:
		WallActor(class Game* game)
			:Actor(game)
		{
			MeshRenderer* mr = new MeshRenderer(this);
			Model* model = GetGame()->GetRenderer()->GetModel("Assets/Models/scene/Wall.obj");
			mr->SetMesh(model);

			m_Box = new BoxComponent(this);
			m_Box->SetObjectBox(model->GetBox());
		}

		BoxComponent* GetBox() const { return m_Box; }

	private:
		BoxComponent* m_Box;
	};
}