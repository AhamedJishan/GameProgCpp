#include "Actor.h"

#include "Component.h"

namespace ch3
{
	Actor::Actor(Game* game)
		:m_State	(EActive),
		 m_Position	(Vector2::Zero),
		 m_Rotation	(0.0f),
		 m_Scale	(0.0f),
		 m_Game		(game)
	{
		// TODO: add actor to the game's actor list
	}
	
	Actor::~Actor()
	{
		// TODO: remove actor from the game's actor list

		for (auto component : m_Components)
			delete component;
	}
	
	void Actor::update(float deltaTime)
	{
		updateComponents(deltaTime);
		updateActor(deltaTime);
	}

	void Actor::addComponent(Component* component)
	{
		int updateOrder = component->getUpdateOrder();
		auto it = m_Components.begin();

		for ( ; it != m_Components.end(); it++)
			if (updateOrder < (*it)->getUpdateOrder())
				break;

		m_Components.insert(it, component);
	}

	void Actor::removeComponent(Component* component)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), component);
		if (it != m_Components.end())
			m_Components.erase(it);
	}
	
	void Actor::updateComponents(float deltaTime)
	{
		for (auto component : m_Components)
			component->update(deltaTime);
	}
	
	void Actor::updateActor(float deltaTime)
	{
	}
}