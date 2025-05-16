#include "CameraComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	CameraComponent::CameraComponent(Actor* owner, int width, int height, float nearPlane, float farPlane, float fov)
		:Component(owner),
		m_Width(width),
		m_Height(height),
		m_NearPlane(nearPlane),
		m_FarPlane(farPlane),
		m_FOV(fov),
		m_Position(0),
		m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)))
	{
	}
	
	glm::mat4 CameraComponent::GetViewMatrix()
	{
		glm::vec3 position = m_Owner->GetPosition() + m_Owner->GetRotation() * m_Position;
		glm::vec3 forward = m_Rotation * m_Owner->GetForward();
		glm::vec3 up = m_Rotation * m_Owner->GetUp();
		glm::mat4 view = glm::lookAt(position, position + forward, up);
		return view;
	}
	
	glm::mat4 CameraComponent::GetProjectionMatrix()
	{
		glm::mat4 projection = glm::perspective(glm::radians(m_FOV), ((float)m_Width / (float)m_Height), m_NearPlane, m_FarPlane);
		return projection;
	}
}