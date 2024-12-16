#include "Actor.h"

namespace ch2
{
	Actor::Actor(Game* game)
		: m_Game(game), m_State(EActive)
	{
	}
	Actor::~Actor()
	{

	}

	void Actor::Update(float deltaTime)
	{
		UpdateComponent(deltaTime);
		UpdateActor(deltaTime);
	}
	
	void Actor::UpdateComponent(float deltaTime)
	{
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::AddComponent(Component* component)
	{
		
	}

	void Actor::RemoveComponent(Component* component)
	{

	}
}
