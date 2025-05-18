#include "FollowCamera.h"

#include "Engine/Actor.h"

namespace jLab
{
	FollowCamera::FollowCamera(Actor* owner)
		:CameraComponent(owner)
	{
		m_Offset = glm::vec3(0, 3.5f, -3.5f);
		m_TargetDistance = 3.5f;
		m_ActualPosition = GetCameraPosition();
		m_Velocity = glm::vec3(0);
		m_SpringConstant = 49.0f;
	}
	
	void FollowCamera::Update(float deltaTime)
	{
		float damping = 2.0f * glm::sqrt(m_SpringConstant);
		glm::vec3 idealPosition = GetCameraPosition();
		glm::vec3 diff = m_ActualPosition - idealPosition;
		// F = ma = a = - kx - cv
		glm::vec3 acceleration = -m_SpringConstant * diff - damping * m_Velocity;

		// v = u + at
		m_Velocity += acceleration * deltaTime;
		// s = vt
		m_ActualPosition += m_Velocity * deltaTime;


		glm::vec3 target = m_Owner->GetPosition() + m_Owner->GetForward() * m_TargetDistance;

		glm::mat4 view = glm::lookAt(m_ActualPosition, target, glm::vec3(0, 1, 0));

		SetViewMatrix(view);
	}

	glm::vec3 FollowCamera::GetCameraPosition()
	{
		glm::vec3 position = m_Owner->GetPosition();
		position = position + m_Owner->GetRight() * m_Offset.x;
		position = position + m_Owner->GetUp() * m_Offset.y;
		position = position + m_Owner->GetForward() * m_Offset.z;

		return position;
	}

	void FollowCamera::SnapToIdeal()
	{
		m_Velocity = glm::vec3(0);
		m_ActualPosition = GetCameraPosition();
		glm::vec3 target = m_Owner->GetPosition() + m_Owner->GetForward() * m_TargetDistance;
		glm::mat4 view = glm::lookAt(m_ActualPosition, target, glm::vec3(0, 1, 0));
		SetViewMatrix(view);
	}
}