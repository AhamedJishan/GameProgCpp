#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class SceneActor : public Actor
	{
	public:
		SceneActor(class Game* game);
		~SceneActor();

		void UpdateActor(float deltaTime) override;
	};
}