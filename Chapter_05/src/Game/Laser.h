#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class Laser : public Actor
	{
	public:
		Laser(class Game* game);

		void UpdateActor(float deltaTime) override;

	private:
		class CircleComponent* m_Circle;
		float m_DeathTimer;
	};
}