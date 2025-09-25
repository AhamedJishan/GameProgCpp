#include "FollowCamera.h"

#include <glm/mat4x4.hpp>
#include "Engine/Actor.h"

namespace jLab
{
	FollowCamera::FollowCamera(Actor* owner)
		:CameraComponent(owner)
		,m_Offset(0.0f, 0.75f, -0.5f)
		,m_TargetDistance(2.0f)
		,m_SpringConstant(100.0f)
		,m_Velocity(glm::vec3(0))
		,m_ActualPos(ComputeCameraPos())
	{
	}

	void FollowCamera::Update(float deltaTime)
	{
		CameraComponent::Update(deltaTime);

		float dampening = 2.0f * glm::sqrt(m_SpringConstant);

		glm::vec3 idealPos = ComputeCameraPos();
		// this refers to the displacement from the acutal position
		glm::vec3 diff = m_ActualPos - idealPos;
		glm::vec3 acceleration = -m_SpringConstant * diff - dampening * m_Velocity;
		m_Velocity = m_Velocity + acceleration * deltaTime;
		m_ActualPos += m_Velocity * deltaTime;

		glm::vec3 targetPos = m_Owner->GetPosition();
		targetPos += m_Owner->GetForward() * m_TargetDistance;
		glm::mat4 view = glm::lookAt(m_ActualPos, targetPos, glm::vec3(0, 1, 0));

		SetViewMatrix(view);
	}
	
	void FollowCamera::SnapToIdeal()
	{
		m_Velocity = glm::vec3(0);

		m_ActualPos = ComputeCameraPos();

		glm::vec3 targetPos = m_Owner->GetPosition();
		targetPos += m_Owner->GetForward() * m_TargetDistance;

		glm::mat4 viewMat = glm::lookAt(m_ActualPos, targetPos, glm::vec3(0, 1, 0));

		SetViewMatrix(viewMat);
	}
	
	glm::vec3 FollowCamera::ComputeCameraPos()
	{
		glm::vec3 position = m_Owner->GetPosition();
		position = position + m_Owner->GetRight() * m_Offset.x;
		position = position + m_Owner->GetUp() * m_Offset.y;
		position = position + m_Owner->GetForward() * m_Offset.z;

		return position;
	}
}