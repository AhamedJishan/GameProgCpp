#pragma once

#include "CameraComponent.h"

namespace jLab
{
	class FPSCamera : public CameraComponent
	{
	public:
		FPSCamera(class Actor* owner);

		void Update(float deltaTime) override;

		void SetPitchSpeed(float pitchSpeed) { m_PitchSpeed = pitchSpeed; }

		float GetPitch() const { return m_Pitch; }

	private:
		const float m_MaxPitch = glm::radians(60.0f);
		float m_PitchSpeed;
		float m_Pitch;
	};
}