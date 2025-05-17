#include "FPSCamera.h"

#include "Engine/Actor.h"

namespace jLab
{
	FPSCamera::FPSCamera(Actor* owner)
		:CameraComponent(owner)
	{
		m_Pitch = 0.0f;
	}

	void FPSCamera::Update(float deltaTime)
	{
		glm::vec3 position = m_Owner->GetPosition();
		glm::vec3 forward = m_Owner->GetForward();
		glm::vec3 up = glm::vec3(0, 1, 0);

		m_Pitch += m_PitchSpeed * deltaTime;
		m_Pitch = glm::clamp(m_Pitch, -m_MaxPitch, m_MaxPitch);
		glm::quat camRotation = glm::angleAxis(m_Pitch, m_Owner->GetRight());

		forward = camRotation * forward;
		up = camRotation * up;

		glm::mat4 view = glm::lookAt(position, position + forward, up);
		SetViewMatrix(view);
	}
}