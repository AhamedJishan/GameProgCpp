#pragma once

#include "CameraComponent.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class OrbitCamera : public CameraComponent
	{
	public:
		OrbitCamera(class Actor* owner);

		void Update(float deltaTime) override;

		void AddYawSpeed(const float yawSpeed) { m_Yaw += yawSpeed; }
		void AddPitchSpeed(const float pitchSpeed) 
		{ 
			m_Pitch += pitchSpeed;
			m_Pitch = glm::clamp(m_Pitch, -glm::radians(80.0f), -glm::radians(5.0f));
		}

	private:
		float m_OffsetDistance;
		glm::vec3 m_Up;
		float m_Yaw;
		float m_Pitch;
	};
}