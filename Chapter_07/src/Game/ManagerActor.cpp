#include "ManagerActor.h"

#include <SDL/SDL.h>
#include "Engine/Component/AudioComponent.h"
#include "Engine/Game.h"


namespace jLab
{
	ManagerActor::ManagerActor(Game* game)
		:Actor(game),
		m_AudioComponent(nullptr)
	{
		m_AudioComponent = new AudioComponent(this);
	}

	ManagerActor::~ManagerActor()
	{
	}

	void ManagerActor::InputActor(const uint8_t* keyState)
	{
	}

	void ManagerActor::UpdateActor(float deltaTime)
	{
	}
}