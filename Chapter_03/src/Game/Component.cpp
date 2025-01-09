#include "Component.h"

namespace ch3
{
	ch3::Component::Component(Actor* owner, int updateOrder)
		:m_Owner(owner),
		m_UppdateOrder(updateOrder)
	{
	}
	
	Component::~Component()
	{
	}
	
	void Component::update(float deltaTime)
	{
	}
}