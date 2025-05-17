#include "MoveComponent.h"

#include "Engine/Actor.h"

namespace jLab
{
	MoveComponent::MoveComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
		m_Velocity = glm::vec3(0);
		m_AngularSpeed = 0.0f;
	}

	void MoveComponent::Update(float deltaTime)
	{
		if (glm::length(m_Velocity) > 0.1f)
		{
			glm::vec3 position = m_Owner->GetPosition();
			position += m_Owner->GetRight() * m_Velocity.x * deltaTime;
			position += m_Owner->GetUp() * m_Velocity.y * deltaTime;
			position += m_Owner->GetForward() * m_Velocity.z * deltaTime;

			m_Owner->SetPosition(position);
		}

		if (m_AngularSpeed != 0)
			m_Owner->Rotate(m_AngularSpeed * deltaTime, glm::vec3(0, 1, 0));
	}
}