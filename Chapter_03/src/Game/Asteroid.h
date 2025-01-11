#pragma once

#include "Actor.h"

namespace ch3
{
	class Asteroid : public Actor
	{
	public:
		Asteroid(class Game* game);
		~Asteroid();

		const class CircleComponent* getCircle() { return m_Circle; }

	private:
		class CircleComponent* m_Circle;
	};
}