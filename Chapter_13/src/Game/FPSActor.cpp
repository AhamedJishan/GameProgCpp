#include "FPSActor.h"

#include "Engine/Game.h"
#include "Engine/InputSystem.h"
#include "Engine/Component/FPSCameraComponent.h"
#include "Engine/Component/MoveComponent.h"

namespace jLab
{
	FPSActor::FPSActor(Game* game)
		:Actor(game)
	{
		mMoveComponent = new MoveComponent(this);
		mCamerComponent = new FPSCameraComponent(this);

		mMoveDir = glm::vec3(0);
	}

	void FPSActor::Input(InputState inputState)
	{
		mMoveDir = glm::vec3(0);
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_W)) mMoveDir.z += 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_S)) mMoveDir.z -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_A)) mMoveDir.x -= 1.0f;
		if (inputState.Keyboard.GetKey(SDL_SCANCODE_D)) mMoveDir.x += 1.0f;

		if (glm::length(mMoveDir) > 0.1f)
			mMoveDir = glm::normalize(mMoveDir);

		glm::vec2 deltaMouse = inputState.Mouse.GetRelativePosition();
		const float maxMouseSpeed = 50.0f;

		float yaw = 0.0f, pitch = 0.0f;

		if (deltaMouse.x != 0)
			yaw += deltaMouse.x / maxMouseSpeed * mAngularSpeed;
		if (deltaMouse.y != 0)
			pitch += deltaMouse.y / maxMouseSpeed * mAngularSpeed;

		mMoveComponent->SetVelocity(mMoveDir * mSpeed);
		mMoveComponent->SetAngularVelocity(-yaw);
		mCamerComponent->SetPitchSpeed(-pitch);
	}

	void FPSActor::Update(float deltaTime)
	{
	}
}