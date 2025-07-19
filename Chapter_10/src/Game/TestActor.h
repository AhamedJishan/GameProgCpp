#pragma once

#include "Engine/Actor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"

#include "Engine/InputSystem.h"
#include <iostream>

namespace jLab
{
	class TestActor : public Actor
	{
	public:
		TestActor(class Game* game)
			:Actor(game)
		{
			MeshRenderer* mr = new MeshRenderer(this);
			mr->SetMesh(m_Game->GetRenderer()->GetModel("Assets/Models/scene/scene.obj"));
			SetPosition(glm::vec3(0, -1, 0));
		}
	};
}