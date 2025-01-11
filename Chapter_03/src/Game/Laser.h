#pragma once

#include "Actor.h"

namespace ch3
{
	class Laser : public Actor
	{
	public:
		Laser(class Game* game);
		~Laser();

		void updateActor(float deltaTime) override;

	private:
		class CircleComponent* m_Circle;
		float m_DeathTimer;
	};
}