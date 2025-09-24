#pragma once

#include "CameraComponent.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class FollowCamera : public CameraComponent
	{
	public:
		FollowCamera(class Actor* owner);

		void Update(float deltaTime) override;

		void SnapToIdeal();
		void SetOffset(const glm::vec3& offset) { m_Offset = offset; }
		void SetTargetDistance(float distance) { m_TargetDistance = m_TargetDistance; }
		void SetSpringConstant(float springConstant) { m_SpringConstant = springConstant; }

	private:
		glm::vec3 ComputeCameraPos();

	private:
		glm::vec3 m_ActualPos;
		glm::vec3 m_Velocity;
		glm::vec3 m_Offset;

		float m_TargetDistance;
		float m_SpringConstant;
	};
}