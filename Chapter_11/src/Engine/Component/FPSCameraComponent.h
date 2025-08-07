#pragma once

#include "CameraComponent.h"
#include <glm/glm.hpp>

namespace jLab
{
	class FPSCameraComponent : public CameraComponent
	{
	public:
		FPSCameraComponent(class Actor* owner, int updateOrder = 100);

		void Update(float deltaTime) override;

		float GetPitch() const { return m_Pitch; }
		void SetPitchSpeed(float pitchSpeed) { m_PitchSpeed = pitchSpeed; }

	private:
		const float m_MaxPitch = glm::radians(60.0f);
		float m_Pitch;
		float m_PitchSpeed;
	};
}