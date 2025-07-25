#pragma once

#include "Engine/Actor.h"
#include <glm/vec3.hpp>

namespace jLab
{
	class FPSActor : public Actor
	{
	public:
		FPSActor(class Game* game);

		void ActorInput(struct InputState& inputState) override;
		void ActorUpdate(float deltaTime) override;

		void FixCollisions();
		void Shoot();

	private:
		class BoxComponent* m_BoxComponent;
		class MoveComponent* m_MoveComponent;
		class FPSCameraComponent* m_FPSCameraComponent;

		glm::vec3 m_MoveDir;
		const float m_Speed = 5.0f;
		const float m_AngularSpeed = 8.0f * 3.14f;
	};
}