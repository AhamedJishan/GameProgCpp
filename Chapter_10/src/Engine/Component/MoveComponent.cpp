#include "MoveComponent.h"

#include "Engine/Actor.h"

namespace jLab
{
	MoveComponent::MoveComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
		m_Velocity = glm::vec3(0);
		m_AngularVelocity = 0.0f;
	}

	void MoveComponent::Update(float deltaTime)
	{
		if (glm::length(m_Velocity) > 0.1f)
		{
			glm::vec3 position = m_Owner->GetPosition();
			position += m_Owner->GetForward() * m_Velocity.z * deltaTime;
			position += m_Owner->GetRight() * m_Velocity.x * deltaTime;
			position += m_Owner->GetUp() * m_Velocity.y * deltaTime;
			m_Owner->SetPosition(position);
		}

		if (glm::epsilonEqual(m_AngularVelocity, 0.0f, 0.1f))
			m_Owner->Rotate(m_AngularVelocity, m_Owner->GetRight());
	}
}