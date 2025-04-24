#pragma once
#include "Engine/Actor.h"


namespace jLab
{
	class HealthBarActor : public Actor
	{
	public:
		HealthBarActor(class Game* game);
		~HealthBarActor();

		void UpdateActor(float deltaTime) override;
	};
}