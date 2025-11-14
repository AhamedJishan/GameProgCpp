#pragma once

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Model.h"
#include "Engine/Skeleton.h"
#include "Engine/Animation.h"
#include "Engine/Component/SkinnedMeshComponent.h"

namespace jLab
{
	class AnimTestActor : public Actor
	{
	public:
		AnimTestActor(class Game* game)
			:Actor(game)
		{
			SkinnedMeshComponent* mc = new SkinnedMeshComponent(this);

			Skeleton* skeleton = mGame->GetSkeleton("Assets/Models/archer/archer_fixed.fbx");
			Model* model = mGame->GetRenderer()->GetModel("Assets/Models/archer/archer_fixed.fbx", true, skeleton);
			Animation* animation = mGame->GetAnimation("Assets/Models/archer/archer_fixed.fbx", skeleton);

			mc->SetMesh(model);
			mc->SetSpecular(glm::vec3(0.3f), 16);
			mc->SetSkeleton(skeleton);
			mc->PlayAnimation(animation);
		}
	};
}