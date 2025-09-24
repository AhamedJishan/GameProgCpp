#include "FollowCamera.h"

#include <glm/mat4x4.hpp>
#include "Engine/Actor.h"

namespace jLab
{
	FollowCamera::FollowCamera(Actor* owner)
		:CameraComponent(owner)
		,m_Offset(0.0f, 1.5f, -3.5f)
		,m_TargetDistance(1.0f)
		,m_SpringConstant(1.28f)
		,m_Velocity(0.0f)
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
		glm::vec3 pos = m_Owner->GetPosition();
		pos += m_Owner->GetForward() * m_Offset.z;
		pos += glm::vec3(0, 1, 0) * m_Offset.z;

		return pos;
	}
}