#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class ContainerActor : public Actor
	{
	public:
		ContainerActor(class Game* game);
		~ContainerActor();

		void InputActor(const uint8_t* keyState) override;
		void UpdateActor(float deltaTime) override;

	private:
		class AudioComponent* m_explosionAudio;
		bool m_SpaceKeyDown;
	};
}