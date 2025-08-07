#include "BoxComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/PhysWorld.h"

namespace jLab
{
	BoxComponent::BoxComponent(Actor* owner, int updateOrder)
		: Component(owner, updateOrder)
		, m_ObjectBox(glm::vec3(0), glm::vec3(0))
		, m_WorldBox(glm::vec3(0), glm::vec3(0))
		, m_ShouldRotate(true)
	{
		m_Owner->GetGame()->GetPhysWorld()->AddBox(this);
	}

	BoxComponent::~BoxComponent()
	{
		m_Owner->GetGame()->GetPhysWorld()->RemoveBox(this);
	}
	
	void BoxComponent::OnUpdateWorldTransform()
	{
		// Reset to object box
		m_WorldBox = m_ObjectBox;
		// SCALE
		m_WorldBox.m_Min.x *= m_Owner->GetScale().x;
		m_WorldBox.m_Min.y *= m_Owner->GetScale().y;
		m_WorldBox.m_Min.z *= m_Owner->GetScale().z;
		m_WorldBox.m_Max.x *= m_Owner->GetScale().x;
		m_WorldBox.m_Max.y *= m_Owner->GetScale().y;
		m_WorldBox.m_Max.z *= m_Owner->GetScale().z;

		// ROTATE
		if (m_ShouldRotate)
			m_WorldBox.Rotate(m_Owner->GetRotation());

		// TRANSLATE
		m_WorldBox.m_Min += m_Owner->GetPosition();
		m_WorldBox.m_Max += m_Owner->GetPosition();
	}
}