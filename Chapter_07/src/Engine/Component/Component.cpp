#include "Component.h"

#include "Engine/Actor.h"

namespace jLab
{
	Component::Component(Actor* owner, int updateOrder)
		:m_Owner(owner),
		m_UpdateOrder(updateOrder)
	{
		m_Owner->AddComponent(this);
	}
	
	Component::~Component()
	{
		m_Owner->RemoveComponent(this);
	}
	
	void Component::ProcessInput(const uint8_t* keyState)
	{
	}
	void Component::Update(float deltaTime)
	{
	}
	void Component::OnUpdateWorldTransform()
	{
	}
}