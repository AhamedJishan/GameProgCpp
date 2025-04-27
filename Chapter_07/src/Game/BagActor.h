#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class BagActor : public Actor
	{
	public:
		BagActor(class Game* game);
		~BagActor();

		void UpdateActor(float deltaTime) override;
	};
}