#pragma once

#include "Actor.h"
#include "SoundEvent.h"

namespace jLab
{
	class Camera : public Actor
	{
	public:
		Camera(class Game* game, int width, int height, float nearPlane, float farPlane, float fov);
		~Camera();

		void InputActor(const uint8_t* keyState) override;
		void UpdateActor(float deltaTime) override;

		glm::mat4 GetViewMatrix();
		glm::mat4 GetViewProjMatrix();
		glm::mat4 GetOrthoProjMatrix();

	private:
		int m_Width;
		int m_Height;
		float m_NearPlane;
		float m_FarPlane;
		float m_FOV;

	private:
		class AudioComponent* m_AudioComponent;
		SoundEvent m_FootstepSound;
		float m_LastFootstep;
		const float m_FOOTSTEP_INTERVAL = 0.5f;
	};
}