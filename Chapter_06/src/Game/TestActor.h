#pragma once

#include "Engine/Actor.h"


namespace jLab
{
	class TestActor : public Actor
	{
	public:
		TestActor(class Game* game);
		~TestActor();

		void UpdateActor(float deltaTime) override;
	};
}