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

		void HitTarget();

	private:
		class MeshRenderer* m_MeshRenderer;
		class BallMoveComponent* m_BallMove;
		class AudioComponent* m_AudioComponent;
		float m_LifeSpan;
	};
}