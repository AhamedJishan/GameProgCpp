#pragma once

#include "Engine/Actor.h"

#include "Engine/Component/SkinnedMeshRenderer.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Skeleton.h"
#include "Engine/Animation.h"

namespace jLab
{
	class AnimationTest : public Actor
	{
	public:
		AnimationTest(class Game* game)
			:Actor(game)
		{
			SkinnedMeshRenderer* smr = new SkinnedMeshRenderer(this);
			Skeleton* skeleton = m_Game->GetSkeleton("Assets/Models/archer/archer_fixed.fbx");
			Model* model = m_Game->GetRenderer()->GetModel("Assets/Models/archer/archer_fixed.fbx", skeleton);
			Animation* animation = m_Game->GetAnimation("Assets/Models/archer/archer_fixed.fbx", skeleton);

			smr->SetMesh(model);
			smr->SetSkeleton(skeleton);
			smr->SetSpecularColor(glm::vec3(0.3f), 16);

			SetScale(glm::vec3(0.005f));
			SetPosition(glm::vec3(0, 0.25f, -5));

			smr->PlayAnimation(animation);
		}
	};
}