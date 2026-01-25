#pragma once

#include "Component.h"

#include <glm/mat4x4.hpp>

namespace jLab
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(class Actor* owner, int updateOrder = 100);

	protected:
		void SetViewMatrix(const glm::mat4& view);
	};
}