#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class ContainerActor : public Actor
	{
	public:
		ContainerActor(class Game* game);
		~ContainerActor();

		void UpdateActor(float deltaTime) override;
	};
}