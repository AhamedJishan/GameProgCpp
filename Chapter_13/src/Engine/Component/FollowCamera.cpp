#include "FollowCamera.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Actor.h"

namespace jLab
{
	FollowCamera::FollowCamera(Actor* owner)
		:CameraComponent(owner)
		,mActualPos(ComputeIdealCamPosition())
		,mVelocity(glm::vec3(0))
		,mOffset(glm::vec3(0, 0.75f, 0.5f))
		,mTargetDistance(-2.0f)
		,mSpringConstant(100.0f)
	{
		SnapToIdeal();
	}

	void FollowCamera::Update(float deltaTime)
	{
		CameraComponent::Update(deltaTime);

		float dampening = 2.0f * glm::sqrt(mSpringConstant);

		glm::vec3 idealPos = ComputeIdealCamPosition();
		glm::vec3 diff = mActualPos - idealPos;
		glm::vec3 accelaration = -mSpringConstant * diff - dampening * mVelocity;
		
		mVelocity += accelaration * deltaTime;
		mActualPos += mVelocity * deltaTime;

		glm::vec3 target = mOwner->GetPosition();
		target += mOwner->GetForward() * mTargetDistance;

		glm::mat4 lookMat = glm::lookAt(mActualPos, target, glm::vec3(0, 1, 0));
		SetViewMatrix(lookMat);
	}
	
	void FollowCamera::SnapToIdeal()
	{
		mActualPos = ComputeIdealCamPosition();
		mVelocity = glm::vec3(0);

		glm::vec3 target = mOwner->GetPosition();
		target += mOwner->GetForward() * mTargetDistance;

		glm::mat4 lookMat = glm::lookAt(mActualPos, target, glm::vec3(0, 1, 0));

		SetViewMatrix(lookMat);
	}
	
	glm::vec3 FollowCamera::ComputeIdealCamPosition()
	{
		glm::vec3 position = mOwner->GetPosition();

		position += mOwner->GetForward() * mOffset.z;
		position += mOwner->GetRight() * mOffset.x;
		position += mOwner->GetUp() * mOffset.y;

		return position;
	}
}