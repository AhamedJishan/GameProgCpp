#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class BallActor : public Actor
	{
	public:
		BallActor(class Game* game);

		void ActorUpdate(float deltaTime) override;
		void SetPlayer(Actor* player);

	private:
		class MeshRenderer* m_MeshRenderer;
		class BallMoveComponent* m_BallMove;
		float m_LifeSpan;
	};
}