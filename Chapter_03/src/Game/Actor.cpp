#include "Actor.h"
namespace ch3
{
	Actor::Actor(Game* game)
		:m_State	(EActive),
		 m_Position	(Vector2::Zero),
		 m_Rotation	(0.0f),
		 m_Scale	(0.0f),
		 m_Game		(game)
	{
	}
	
	Actor::~Actor()
	{
	}
	
	void Actor::update(float deltaTime)
	{
		updateComponents(deltaTime);
		updateActor(deltaTime);
	}
	
	void Actor::updateComponents(float deltaTime)
	{
	}
	
	void Actor::updateActor(float deltaTime)
	{
	}
}