#include "Actor.h"

#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_State(State::EActive),
		m_Position(Vector2::Zero),
		m_Scale(1),
		m_Rotation(0)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);

		// TODO: Delete all Actors
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::EActive)
		{
			UpdateComponent(deltaTime);
			UpdateActor(deltaTime);
		}
	}
	
	void Actor::ProcessInput(const uint8_t* keyState)
	{
		if (m_State == State::EActive)
		{
			// TODO: Component input processing
			ActorInput(keyState);
		}
	}
	
	// VIRTUAL
	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::UpdateComponent(float deltaTime)
	{
		// TODO: Update all the components
	}
	
	// VIRTUAL
	void Actor::ActorInput(const uint8_t* keyState)
	{
	}

}