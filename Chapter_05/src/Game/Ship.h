#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class Ship : public Actor
	{
	public:
		Ship(class Game* game);

		void UpdateActor(float deltaTime) override;
		void ActorInput(const uint8_t* keyState) override;

	private:
		float m_LaserCooldown;
	};
}