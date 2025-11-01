#include "FPSCameraComponent.h"

#include "Engine/Actor.h"

namespace jLab
{
	FPSCameraComponent::FPSCameraComponent(Actor* owner, int updateOrder)
		:CameraComponent(owner, updateOrder)
	{
		mPitch = 0.0f;
		mPitchSpeed = 0.0f;
	}

	void FPSCameraComponent::Update(float deltaTime)
	{
		mPitch += mPitchSpeed * deltaTime;
		mPitch = glm::clamp(mPitch, -MAX_PITCH, MAX_PITCH);
		glm::quat rotation = glm::angleAxis(mPitch, mOwner->GetRight());

		glm::vec3 position = mOwner->GetPosition();
		glm::vec3 forward = mOwner->GetForward();
		glm::vec3 up = glm::vec3(0, 1, 0);

		forward = rotation * forward;
		up = rotation * up;

		glm::mat4 view = glm::lookAt(position, position + forward, up);
		SetViewMatrix(view);
	}
}