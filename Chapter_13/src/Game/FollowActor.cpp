#include "FollowActor.h"

#include <glm/vec3.hpp>
#include "Engine/Game.h"
#include "Engine/InputSystem.h"
#include "Engine/Renderer.h"
#include "Engine/Component/MoveComponent.h"
#include "Engine/Component/SkinnedMeshComponent.h"
#include "Engine/Component/FollowCamera.h"
#include "Engine/Skeleton.h"
#include "Engine/Animation.h"
#include "Engine/Model.h"

namespace jLab
{
	FollowActor::FollowActor(Game* game)
		:Actor(game)
		,mIsMoving(false)
	{
		Skeleton* skeleton = mGame->GetSkeleton("Assets/Models/eve/eve.dae");
		Model* model = mGame->GetRenderer()->GetModel("Assets/Models/eve/eve.dae", false, skeleton);
		mIdleAnim = mGame->GetAnimation("Assets/Models/eve/eve_idle.dae", skeleton);
		mRunAnim = mGame->GetAnimation("Assets/Models/eve/eve_run.dae", skeleton);

		mMoveComp = new MoveComponent(this);
		mFollowCamComp = new FollowCamera(this);
		mSkinnedMeshComp = new SkinnedMeshComponent(this);

		mSkinnedMeshComp->SetMesh(model);
		mSkinnedMeshComp->SetSkeleton(skeleton);
		mSkinnedMeshComp->PlayAnimation(mIdleAnim);

		mFollowCamComp->SetOffset(glm::vec3(0, 0.75f, 0.75f));
		mFollowCamComp->SetTargetDistance(-5.0f);


		SetScale(glm::vec3(0.00006f));
		SetPosition(glm::vec3(0, 0.25f, 0));
		Rotate(glm::radians(180.0f), glm::vec3(0, 1, 0));
	}

	void FollowActor::Input(InputState inputState)
	{
		Actor::Input(inputState);

		glm::vec3 moveDir = glm::vec3(0);
		float angularSpeed = 0;

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_W)) moveDir.z -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_A)) angularSpeed -= mMaxAngularSpeed;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_D)) angularSpeed += mMaxAngularSpeed;

		if (glm::length(moveDir) > 0.1f) moveDir = glm::normalize(moveDir);

		if (!mIsMoving && glm::length(moveDir) > 0.1f)
		{
			mIsMoving = true;
			mSkinnedMeshComp->PlayAnimation(mRunAnim);
		}
		if (mIsMoving && glm::length(moveDir) < 0.1f)
		{
			mIsMoving = false;
			mSkinnedMeshComp->PlayAnimation(mIdleAnim);
		}

		mMoveComp->SetVelocity(moveDir * mMaxSpeed);
		mMoveComp->SetAngularVelocity(-angularSpeed);
	}


}