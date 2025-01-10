#pragma once

#include "Actor.h"

namespace ch3
{
	class Asteroid : public Actor
	{
	public:
		Asteroid(class Game* game);
		~Asteroid();
	};
}