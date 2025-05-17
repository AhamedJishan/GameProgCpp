#include "FollowCamera.h"

#include "Engine/Actor.h"

namespace jLab
{
	FollowCamera::FollowCamera(Actor* owner)
		:CameraComponent(owner)
	{
		m_Offset = glm::vec3(0, 3.5f, -3.5f);
		m_TargetDistance = 3.5f;
		m_Position = glm::vec3(0);
	}
	
	void FollowCamera::Update(float deltaTime)
	{
		glm::vec3 target = m_Owner->GetPosition() + m_Owner->GetForward() * m_TargetDistance;

		glm::mat4 view = glm::lookAt(GetCameraPosition(), target, glm::vec3(0, 1, 0));

		SetViewMatrix(view);
	}
	
	void FollowCamera::ComputeCameraPosition()
	{
		glm::vec3 position = m_Owner->GetPosition();
		position = position + m_Owner->GetRight() * m_Offset.x;
		position = position + m_Owner->GetUp() * m_Offset.y;
		position = position + m_Owner->GetForward() * m_Offset.z;

		m_Position = position;
	}

	glm::vec3 FollowCamera::GetCameraPosition()
	{
		glm::vec3 position = m_Owner->GetPosition();
		position = position + m_Owner->GetRight() * m_Offset.x;
		position = position + m_Owner->GetUp() * m_Offset.y;
		position = position + m_Owner->GetForward() * m_Offset.z;

		return position;
	}
}