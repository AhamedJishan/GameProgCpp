#include "MoveComponent.h"

#include "util/Math.h"
#include "Actor.h"

namespace jLab
{
	MoveComponent::MoveComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder),
		m_AngularSpeed(0),
		m_ForwardSpeed(0)
	{
	}
	
	void MoveComponent::Update(float deltaTime)
	{
		if (!Math::NearZero(m_AngularSpeed))
		{
			float rot = m_Owner->GetRotation();
			rot += m_AngularSpeed * deltaTime;
			m_Owner->SetRotation(rot);
		}

		if (!Math::NearZero(m_ForwardSpeed))
		{
			Vector2 pos = m_Owner->GetPosition();
			pos += m_Owner->GetForward() * m_ForwardSpeed * deltaTime;
			m_Owner->SetPosition(pos);
		}
	}
}