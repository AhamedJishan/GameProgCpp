#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class FollowActor : public Actor
	{
	public:
		FollowActor(class Game* game);

		void Input(struct InputState inputState) override;

	private:
		class MoveComponent* mMoveComp;
		class SkinnedMeshComponent* mSkinnedMeshComp;
		class FollowCamera* mFollowCamComp;

		class Animation* mIdleAnim;
		class Animation* mRunAnim;

		float mMaxSpeed = 2.0f;
		float mMaxAngularSpeed = 2.75f;

		bool mIsMoving;
	};
}