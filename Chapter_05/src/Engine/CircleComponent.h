#pragma once

#include "Component.h"
#include "util/Math.h"

namespace jLab
{
	class CircleComponent : public Component
	{
	public:
		CircleComponent(class Actor* owner, int updateOrder = 100);

		void SetRadius(const float radius) { m_Radius = radius; }
		float GetRadius() const { return m_Radius; }

		const Vector3& GetCenter() const;

	private:
		float m_Radius;
	};

	bool Intersect(const CircleComponent& a, const CircleComponent& b);
}