#include "FPSActor.h"

#include "Engine/Game.h"
#include "Engine/InputSystem.h"
#include "Engine/Component/FPSCameraComponent.h"
#include "Engine/Component/MoveComponent.h"
#include "Engine/Component/BoxComponent.h"
#include "WallActor.h"

namespace jLab
{
	FPSActor::FPSActor(Game* game)
		:Actor(game)
	{
		mMoveComponent = new MoveComponent(this);
		mCamerComponent = new FPSCameraComponent(this);

		mBoxComponent = new BoxComponent(this);
		AABB boxAABB(glm::vec3(-0.25f, 0.0f, -0.25f), glm::vec3(0.25f, 1.0f, 0.25f));
		mBoxComponent->SetObjectBox(boxAABB);
		mBoxComponent->SetShouldRotate(false);

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
		FixCollisions();
	}

	void FPSActor::FixCollisions()
	{
		ComputeWorldTransform();

		const AABB& playerBox = mBoxComponent->GetWorldBox();
		glm::vec3 pos = GetPosition();

		const std::vector<WallActor*>& planes = GetGame()->GetWallPlanes();

		for (WallActor* plane : planes)
		{
			plane->GetBox()->OnUpdateWorldTransform();
			const AABB& planeBox = plane->GetBox()->GetWorldBox();
			if (Intersects(playerBox, planeBox))
			{
				float dx1 = planeBox.m_Min.x - playerBox.m_Max.x;
				float dx2 = planeBox.m_Max.x - playerBox.m_Min.x;
				float dy1 = planeBox.m_Min.y - playerBox.m_Max.y;
				float dy2 = planeBox.m_Max.y - playerBox.m_Min.y;
				float dz1 = planeBox.m_Min.z - playerBox.m_Max.z;
				float dz2 = planeBox.m_Max.z - playerBox.m_Min.z;

				float dx = glm::abs(dx1) < glm::abs(dx2) ? dx1 : dx2;
				float dy = glm::abs(dy1) < glm::abs(dy2) ? dy1 : dy2;
				float dz = glm::abs(dz1) < glm::abs(dz2) ? dz1 : dz2;

				if (glm::abs(dx) <= std::abs(dy) && glm::abs(dx) <= glm::abs(dz))
					pos.x += dx;
				else if (glm::abs(dy) <= std::abs(dx) && glm::abs(dy) <= glm::abs(dz))
					pos.y += dy;
				else
					pos.z += dz;

				SetPosition(pos);
				mBoxComponent->OnUpdateWorldTransform();
			}
		}
	}
}