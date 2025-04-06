#pragma once

#include "Component.h"
#include"util/Math.h"

namespace jLab
{
	class CircleComponent : public Component
	{
	public:
		CircleComponent(class Actor* owner);

		void SetRadius(float radius) { m_Radius = radius; }
		float GetRadius() const { return m_Radius; }

		const Vector2& GetCenter() const;
	private:
		float m_Radius;
	};

	bool Intersect(const CircleComponent& c1, const CircleComponent& c2);
}