#pragma once

#include "Actor.h"

namespace ch3
{
	class Ship : public Actor
	{
	public:
		Ship(class Game* game);
		~Ship();
	};
}