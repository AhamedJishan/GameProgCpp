#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace jLab
{
	struct BoneTransform
	{
		glm::vec3 Translation;
		glm::vec3 Scale;
		glm::quat Rotation;

		glm::mat4 ToMatrix() const;

		static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float t);
		static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float translationT, float rotationT, float scaleT);
	};
}