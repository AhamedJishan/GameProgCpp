#pragma once
#include "Engine/Actor.h"


namespace jLab
{
	class SceneActor : public Actor
	{
	public:
		SceneActor(class Game* game);
		~SceneActor();

		void InputActor(const uint8_t* keyState) override;
		void UpdateActor(float deltaTime) override;
	};
}