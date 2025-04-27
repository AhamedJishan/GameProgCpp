#pragma once

#include "Actor.h"


namespace jLab
{
	class Camera : public Actor
	{
	public:
		Camera(class Game* game, int width, int height, float nearPlane, float farPlane, float fov);
		~Camera();

		void InputActor(const uint8_t* keyState) override;
		void UpdateActor(float deltaTime) override;

		glm::mat4 GetViewProjMatrix();
		glm::mat4 GetOrthoProjMatrix();

	private:
		int m_Width;
		int m_Height;
		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;
	};
}