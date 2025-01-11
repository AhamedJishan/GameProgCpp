#include "CircleComponent.h"
#include "Actor.h"

namespace ch3
{
	CircleComponent::CircleComponent(Actor* owner)
		:Component(owner)
	{
	}

	float CircleComponent::getRadius() const
	{
		return m_Owner->getScale() * m_Radius;
	}

	const Vector2& CircleComponent::getCenter() const
	{
		return m_Owner->getPosition();
	}


	bool intersect(const CircleComponent& a, const CircleComponent& b)
	{
		Vector2 v1 = a.getCenter();
		Vector2 v2 = b.getCenter();
		Vector2 diff = v1 - v2;
		//Vector2 diff = a.getCenter() - b.getCenter();
		float distSq = diff.LengthSq();

		float radiiSq = a.getRadius() + b.getRadius();
		radiiSq *= radiiSq;

		return distSq <= radiiSq;
	}
}