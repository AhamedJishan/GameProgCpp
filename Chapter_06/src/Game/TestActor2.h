#pragma once

#include "Engine/Actor.h"


namespace jLab
{
	class TestActor2 : public Actor
	{
	public:
		TestActor2(class Game* game);
		~TestActor2();

		void UpdateActor(float deltaTime) override;
	};
}