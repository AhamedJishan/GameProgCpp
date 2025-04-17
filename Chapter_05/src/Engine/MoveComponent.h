#pragma once

#include "Component.h"

namespace jLab
{
	class MoveComponent : public Component
	{
	public:
		MoveComponent(class Actor* owner, int updateOrder = 10);

		void Update(float deltaTime) override;

		float GetForwardSpeed() { return m_ForwardSpeed; }
		float GetAngularSpeed() { return m_AngularSpeed; }
		void SetForwardSpeed(const float speed) { m_ForwardSpeed = speed; }
		void SetAngularSpeed(const float speed) { m_AngularSpeed = speed; }

	private:
		float m_ForwardSpeed;
		float m_AngularSpeed;
	};
}