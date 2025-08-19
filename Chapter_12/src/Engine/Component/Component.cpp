#include "Component.h"

#include "Engine/Actor.h"
#include "Engine/InputSystem.h"

namespace jLab
{
	Component::Component(Actor* owner, int updateOrder)
		:m_Owner(owner)
		,m_UpdateOrder(updateOrder)
	{
		m_Owner->AddComponent(this);
	}
	
	Component::~Component()
	{
		m_Owner->RemoveComponent(this);
	}
	
	void Component::Update(float deltaTime)
	{
	}
	
	void Component::Input(InputState& inputState)
	{
	}
	
	void Component::OnUpdateWorldTransform()
	{
	}
}