#include "Actor.h"

#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Position(Vector2::Zero),
		m_Rotation(0),
		m_Scale(0),
		m_Game(game)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::EActive)
		{
			UpdateComponent(deltaTime);
			UpdateActor(deltaTime);
		}
	}
	
	void Actor::UpdateComponent(float deltaTime)
	{
		// Update each component
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::ProcessInput(const uint8_t* keyboardState)
	{
		// pass the keyboard state to each component

		ActorInput(keyboardState);
	}
	
	void Actor::ActorInput(const uint8_t* keyboardState)
	{
	}
}