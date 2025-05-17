#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class MoveComponent : public Component
	{
	public:
		MoveComponent(class Actor* owner, int updateOrder = 100);

		void Update(float deltaTime) override;

		glm::vec3 GetVelocity() const { return m_Velocity; }
		float GetAngularSpeed() const { return m_AngularSpeed; }

		void SetVelocity(const glm::vec3 velocity) { m_Velocity = velocity; }
		void SetAngularSpeed(float angularSpeed) { m_AngularSpeed = angularSpeed; }

	protected:
		glm::vec3 m_Velocity;
		float m_AngularSpeed;
	};
}