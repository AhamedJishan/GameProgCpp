#pragma once

#include "Actor.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace jLab
{
	class Camera : public Actor
	{
	public:
		Camera(class Game* game, int width, int height, float nearPlane, float farPlane, float fov);
		~Camera();

		void Input(struct InputState& inputState) override;
		void Update(float deltaTime) override;

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