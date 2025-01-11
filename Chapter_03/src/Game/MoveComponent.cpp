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

			// Screen Wrapping just for Chapter_03 project.
			if (pos.x < -2.0f) pos.x = 1278.0f;
			else if (pos.x > 1282.0f) pos.x = -2.0f;
			if (pos.y < -2.0f) pos.y = 718.0f;
			else if (pos.y > 722) pos.y = -2.0f;

			m_Owner->setPosition(pos);
		}
	}
}