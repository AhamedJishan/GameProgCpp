#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class ManagerActor : public Actor
	{
	public:
		ManagerActor(class Game* game);
		~ManagerActor();

		void InputActor(const uint8_t* keyState) override;
		void UpdateActor(float deltaTime) override;

	private:
		class AudioComponent* m_AudioComponent;
	};
}