#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class Asteroid : public Actor
	{
	public:
		Asteroid(class Game* game);
		~Asteroid();

		class CircleComponent* GetCircle() { return m_Circle; }

	private:
		class CircleComponent* m_Circle;
	};
}