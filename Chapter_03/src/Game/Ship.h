#pragma once

#include "Actor.h"
#include <stdint.h>

namespace ch3
{
	class Ship : public Actor
	{
	public:
		Ship(class Game* game);
		~Ship();

		void actorInput(const uint8_t* keyState) override;
		void updateActor(float deltaTime) override;

	private:
		float m_LaserCooldown;
	};
}