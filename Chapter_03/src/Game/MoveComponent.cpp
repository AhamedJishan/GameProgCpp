#include "MoveComponent.h"

#include "Math.h"
#include "Actor.h"

namespace ch3
{
	MoveComponent::MoveComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder),
		m_AngularSpeed(0),
		m_ForwardSpeed(0)
	{
	}

	void MoveComponent::update(float deltaTime)
	{
		if (!Math::NearZero(m_AngularSpeed))
		{
			float rot = m_Owner->getRotation();
			rot += m_AngularSpeed * deltaTime;
			m_Owner->setRotation(rot);
		}

		if (!Math::NearZero(m_ForwardSpeed))
		{
			Vector2 pos = m_Owner->getPosition();
			pos += m_Owner->getForward() * m_ForwardSpeed * deltaTime;
			m_Owner->setPosition(pos);
		}
	}
}