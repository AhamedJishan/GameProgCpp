#include "Component.h"

#include "Actor.h"

namespace ch3
{
	ch3::Component::Component(Actor* owner, int updateOrder)
		:m_Owner(owner),
		m_UppdateOrder(updateOrder)
	{
		m_Owner->addComponent(this);
	}
	
	Component::~Component()
	{
		m_Owner->removeComponent(this);
	}
	
	void Component::update(float deltaTime)
	{
	}
	void Component::processInput(const uint8_t* keyState)
	{
	}
}