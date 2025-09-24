#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class FollowActor : public Actor
	{
	public:
		FollowActor(class Game* game);

		void ActorInput(struct InputState& inputState) override;

	private:
		class MoveComponent* m_MoveComp;
		class FollowCamera* m_FollowCam;
		class SkinnedMeshRenderer* m_SkinnedMeshRenderer;

		class Animation* m_IdleAnim;
		class Animation* m_RunAnim;
	};
}