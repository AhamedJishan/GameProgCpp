#include "Camera.h"

#include "Game.h"
#include "Renderer.h"

namespace jLab
{
	Camera::Camera(Game* game, int width, int height, float nearPlane, float farPlane, float fov)
		:Actor(game),
		m_Width(width),
		m_Height(height),
		m_NearPlane(nearPlane),
		m_FarPlane(farPlane),
		m_FOV(fov)
	{
	}
	
	Camera::~Camera()
	{
	}
	
	void Camera::InputActor(const uint8_t* keyState)
	{
		float speed = 0.05f;
		if (keyState[SDL_SCANCODE_W]) SetPosition(GetPosition() + GetForward() * speed);
		if (keyState[SDL_SCANCODE_S]) SetPosition(GetPosition() - GetForward() * speed);
		if (keyState[SDL_SCANCODE_D]) SetPosition(GetPosition() + GetRight() * speed);
		if (keyState[SDL_SCANCODE_A]) SetPosition(GetPosition() - GetRight() * speed);
		if (keyState[SDL_SCANCODE_RIGHT])
		{
			SetRotation(GetRotation() * glm::angleAxis(glm::radians(-speed * 20), glm::vec3(0, 1, 0)));
		}
		if (keyState[SDL_SCANCODE_LEFT])
		{
			SetRotation(GetRotation() * glm::angleAxis(glm::radians(speed * 20), glm::vec3(0, 1, 0)));
		}
	}
	
	void Camera::UpdateActor(float deltaTime)
	{
	}
	
	glm::mat4 Camera::GetViewMatrix()
	{
		glm::vec3 pos = GetPosition();
		glm::vec3 forward = GetForward();
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::mat4 view = glm::lookAt(pos, pos + forward, up);
		return view;
	}

	glm::mat4 Camera::GetViewProjMatrix()
	{
		glm::vec3 pos = GetPosition();
		glm::vec3 forward = GetForward();
		glm::vec3 up = glm::vec3(0, 1, 0);

		glm::mat4 view = glm::lookAt(pos, pos + forward, up);
		glm::mat4 proj = glm::perspective(glm::radians(m_FOV), ((float)m_Width / (float)m_Height), m_NearPlane, m_FarPlane);

		return (proj * view);
	}
	
	glm::mat4 Camera::GetOrthoProjMatrix()
	{
		glm::mat4 proj = glm::ortho((-m_Width / 2.0f), m_Width / 2.0f, (-m_Height / 2.0f), m_Height / 2.0f);
		return proj;
	}
}