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

		void ComputeCameraPosition();
		//glm::vec3 GetCameraPosition() { return m_Position; }
		glm::vec3 GetCameraPosition();

		void SetOffset(const glm::vec3 offset) { m_Offset = offset; }
		void SetTargetDistance(const float targetDistance) { m_TargetDistance = targetDistance; }

	private:
		glm::vec3 m_Offset;
		float m_TargetDistance;

		glm::vec3 m_Position;
	};
}