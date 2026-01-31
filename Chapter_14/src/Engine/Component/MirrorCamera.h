#pragma once

#include "CameraComponent.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class MirrorCamera : public CameraComponent
	{
	public:
		MirrorCamera(class Actor* owner);

		void Update(float deltaTime) override;

		void SnapToIdeal();

		void SetOffset(glm::vec3 offset) { mOffset = offset; }
		void SetTargetDistance(float targetDistance) { mTargetDistance = targetDistance; }
		void SetSpringConstant(float springConstant) { mSpringConstant = springConstant; }

		Component::TypeID GetType() const override { return Component::TMirrorCameraComponent; }

	private:
		glm::vec3 ComputeIdealCamPosition();

	private:
		glm::vec3 mActualPos;
		glm::vec3 mVelocity;
		glm::vec3 mOffset;

		float mTargetDistance;
		float mSpringConstant;
	};
}