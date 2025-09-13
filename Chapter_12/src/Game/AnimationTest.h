#pragma once

#include "Engine/Actor.h"

#include "Engine/Component/MeshRenderer.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Skeleton.h"

namespace jLab
{
	class AnimationTest : public Actor
	{
	public:
		AnimationTest(class Game* game)
			:Actor(game)
		{
			MeshRenderer* mr = new MeshRenderer(this);
			Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/archer/archer_fixed.fbx");
			mr->SetMesh(model);
			mr->SetSpecularColor(glm::vec3(0.3f), 16);
			SetScale(glm::vec3(0.005f));
			SetPosition(glm::vec3(0, 0.25f, 0));
		}
	};
}