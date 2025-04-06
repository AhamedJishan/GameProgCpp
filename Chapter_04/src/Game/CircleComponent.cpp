#include "CircleComponent.h"

#include "Actor.h"

namespace jLab
{
	CircleComponent::CircleComponent(Actor* owner)
		:Component(owner)
		,m_Radius(0)
	{
	}
	
	const Vector2& CircleComponent::GetCenter() const
	{
		return m_Owner->GetPosition();
	}
	
	bool Intersect(const CircleComponent& c1, const CircleComponent& c2)
	{
		Vector2 diff = c1.GetCenter() - c2.GetCenter();
		float distSq = diff.LengthSq();

		float radiiSq = c1.GetRadius() + c2.GetRadius();
		radiiSq *= radiiSq;
		return distSq <= radiiSq;
	}
}