#include "OrbitCamera.h"

#include <glm/gtc/quaternion.hpp>
#include "Engine/Actor.h"

namespace jLab
{
	OrbitCamera::OrbitCamera(Actor* owner)
		:CameraComponent(owner)
	{
		m_OffsetDistance = 4.0f;
		m_Up = glm::vec3(0, 1, 0);
		m_Pitch = 0.0f;
		m_Yaw= 0.0f;
	}
	
	void OrbitCamera::Update(float deltaTime)
	{
		glm::vec3 offset = glm::vec3(0, 0, m_OffsetDistance);
		glm::vec3 up = m_Up;

		// Forward = owner.position - (owner.position + offset)
		// Forward = -offset
		glm::vec3 forward = -offset;
		forward = glm::normalize(forward);
		glm::vec3 right = glm::cross(forward, up);
		right = glm::normalize(right);

		glm::quat yaw = glm::angleAxis(m_Yaw, glm::vec3(0, 1, 0));
		glm::quat pitch = glm::angleAxis(m_Pitch, right);
		glm::quat orientation = yaw * pitch;

		offset = orientation * offset;
		up = orientation * up;

		glm::vec3 target = m_Owner->GetPosition();
		glm::vec3 position = target + offset;

		glm::mat4 view = glm::lookAt(position, target, up);

		SetViewMatrix(view);
	}
}