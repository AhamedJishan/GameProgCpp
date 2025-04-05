#pragma once

#include "Component.h"

namespace jLab
{
	class MoveComponent : public Component
	{
	public:
		MoveComponent(class Actor* owner, int updateOrder = 100);

		void Update(float deltaTime) override;

		float GetAngularSpeed() { return m_AngularSpeed; }
		float GetForwardSpeed() { return m_ForwardSpeed; }
		void SetAngularSpeed(float angularSpeed) { m_AngularSpeed = angularSpeed; }
		void SetForwardSpeed(float forwardSpeed) { m_ForwardSpeed = forwardSpeed; }

	private:
		float m_AngularSpeed;
		float m_ForwardSpeed;
	};
}