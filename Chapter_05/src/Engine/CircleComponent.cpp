#include "CircleComponent.h"

#include "Actor.h"

namespace jLab
{
	CircleComponent::CircleComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder),
		m_Radius(0)
	{
	}
	
	const Vector3& CircleComponent::GetCenter() const
	{
		return m_Owner->GetPosition();
	}
	
	bool Intersect(const CircleComponent& a, const CircleComponent& b)
	{
		float radiiSq = a.GetRadius() + b.GetRadius();
		radiiSq *= radiiSq;

		Vector3 diff = a.GetCenter() - b.GetCenter();
		float distSq = diff.LengthSq();

		return radiiSq < distSq;
	}
}