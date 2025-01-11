#pragma once

#include "Component.h"
#include "Math.h"

namespace ch3
{
	class CircleComponent : public Component
	{
	public:
		CircleComponent(class Actor* owner);

		float getRadius() const;
		void setRadius(float radius) { m_Radius = radius; }

		const Vector2& getCenter() const;

	private:
		float m_Radius;
	};

	bool intersect(const CircleComponent& a, const CircleComponent& b);
}