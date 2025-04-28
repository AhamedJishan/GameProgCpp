#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class RadarActor : public Actor
	{
	public:
		RadarActor(class Game* game);
		~RadarActor();

		void UpdateActor(float deltaTime) override;
	};
}