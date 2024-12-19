#pragma once

#include "Actor.h"

namespace ch2
{
	class Ship : public Actor
	{
	public:
		Ship(class Game* game);

		void UpdateActor(float deltaTime) override;

		void ProcessInput(const uint8_t* state);

		float GetSpeedX() { return m_SpeedX; }
		float GetSpeedY() { return m_SpeedY; }

	private:
		float m_SpeedX;
		float m_SpeedY;
	};
}

