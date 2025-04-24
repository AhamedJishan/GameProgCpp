#pragma once

#include "Engine/Actor.h"


namespace jLab
{
	class BackPackActor : public Actor
	{
	public:
		BackPackActor(class Game* game);
		~BackPackActor();

		void UpdateActor(float deltaTime) override;
	};
}