#pragma once

#include "Engine/Actor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"

namespace jLab
{
	class TestActor : public Actor
	{
	public:
		TestActor(class Game* game)
			:Actor(game)
		{
			MeshRenderer* mr = new MeshRenderer(this);
			mr->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/scene/GroundPlane.obj"));
		}
	};
}