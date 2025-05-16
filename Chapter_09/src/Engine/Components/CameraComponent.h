#pragma once

#include "Component.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace jLab
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(class Actor* owner, int width, int height, float nearPlane, float farPlane, float fov);

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		float GetNearPlane() const { return m_NearPlane; }
		float GetFarPlane() const { return m_FarPlane; }
		float GetFOV() const { return m_FOV; }
		glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetRotation() const { return m_Rotation; }

		void SetWidth(int width) { m_Width = width; }
		void SetHeight(int height) { m_Height = height; }
		void SetNearPlane(float nearPlane) { m_NearPlane = nearPlane; }
		void SetFarPlane(float farPlane) { m_FarPlane = farPlane; }
		void SetFOV(float fov) { m_FOV = fov; }
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetRotation(glm::quat rotation) { m_Rotation = rotation; }
		void Rotate(float angle, glm::vec3 axis)
		{
			glm::quat rot = glm::angleAxis(glm::radians(angle), axis);
			m_Rotation = glm::normalize(m_Rotation * rot);
		}
		
	protected:
		int m_Width;
		int m_Height;
		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;

		glm::vec3 m_Position;
		glm::quat m_Rotation;
	};
}