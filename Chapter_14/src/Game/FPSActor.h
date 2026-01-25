#pragma once

#include "Engine/Actor.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class FPSActor : public Actor
	{
	public:
		FPSActor(class Game* game);

		void Input(struct InputState inputState) override;
		void Update(float deltaTime) override;

	private:
		void FixCollisions();

	private:
		glm::vec3 mMoveDir;
		const float mSpeed = 2.0f;
		const float mAngularSpeed = 8 * 3.14f;

		class MoveComponent* mMoveComponent;
		class BoxComponent* mBoxComponent;
		class FPSCameraComponent* mCamerComponent;
	};
}