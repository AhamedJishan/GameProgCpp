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

		glm::vec3 GetCameraPosition();
		void SnapToIdeal();

		void SetOffset(const glm::vec3& offset) { m_Offset = offset; }
		void SetTargetDistance(const float targetDistance) { m_TargetDistance = targetDistance; }

	private:
		glm::vec3 m_Offset;
		float m_TargetDistance;

		glm::vec3 m_ActualPosition;
		glm::vec3 m_Velocity;
		float m_SpringConstant;
	};
}