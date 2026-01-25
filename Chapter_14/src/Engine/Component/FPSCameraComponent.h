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

		void SetPitchSpeed(float pitchSpeed) { mPitchSpeed = pitchSpeed; }
		float GetPitchSpeed() const { return mPitchSpeed; }

	private:
		const float MAX_PITCH = glm::radians(60.0f);
		float mPitch;
		float mPitchSpeed;
	};
}