#include "MoveComponent.h"

#include "Engine/Game.h"
#include "Engine/Actor.h"

namespace jLab
{
	MoveComponent::MoveComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
		mVelocity = glm::vec3(0);
		mAngularVelocity = 0.0f;
	}

	void MoveComponent::Update(float deltaTime)
	{
		if (glm::length(mVelocity) != 0.0f)
		{
			glm::vec3 position = mOwner->GetPosition();
			position += mOwner->GetForward() * mVelocity.z * deltaTime;
			position += mOwner->GetRight() * mVelocity.x * deltaTime;
			position += mOwner->GetUp() * mVelocity.y * deltaTime;
			mOwner->SetPosition(position);
		}

		if (mAngularVelocity != 0.0f)
			mOwner->Rotate(mAngularVelocity * deltaTime, glm::vec3(0, 1, 0));
	}
}