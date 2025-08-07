#include "FPSCameraComponent.h"

#include "Engine/Actor.h"

namespace jLab
{
	FPSCameraComponent::FPSCameraComponent(Actor* owner, int updateOrder)
		:CameraComponent(owner, updateOrder)
	{
		m_Pitch = 0.0f;
		m_PitchSpeed = 0.0f;
	}
	
	void FPSCameraComponent::Update(float deltaTime)
	{
		m_Pitch += m_PitchSpeed * deltaTime;
		m_Pitch = glm::clamp(m_Pitch, -m_MaxPitch, m_MaxPitch);
		glm::quat rotation = glm::angleAxis(m_Pitch, m_Owner->GetRight());

		glm::vec3 position = m_Owner->GetPosition();
		glm::vec3 forward = m_Owner->GetForward();
		glm::vec3 up = glm::vec3(0, 1, 0);

		forward = rotation * forward;
		up = rotation * up;

		glm::mat4 view = glm::lookAt(position, position + forward, up);
		SetViewMatrix(view);
	}
}