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
			float rotation = m_Owner->GetRotation();
			rotation += m_AngularSpeed * deltaTime;
			m_Owner->SetRotation(rotation);
		}
		
		if (!Math::NearZero(m_ForwardSpeed))
		{
			Vector3 pos = m_Owner->GetPosition();
			pos += m_Owner->GetForward() * m_ForwardSpeed * deltaTime;

			// Wrap around the screen(1280x720)
			if (pos.x > 660)
				pos.x = -655;
			else if (pos.x < -660)
				pos.x = 655;
			else if (pos.y > 380)
				pos.y = -375;
			else if (pos.y < -380)
				pos.y = 375;

			m_Owner->SetPosition(pos);
		}
	}
}