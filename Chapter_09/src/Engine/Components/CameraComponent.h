#pragma once

#include "Component.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace jLab
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(class Actor* owner);
	protected:
		void SetViewMatrix(const glm::mat4 view);
	};
}