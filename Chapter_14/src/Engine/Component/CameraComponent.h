#pragma once

#include "Component.h"

#include <glm/mat4x4.hpp>

namespace jLab
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(class Actor* owner, int updateOrder = 100);

		Component::TypeID GetType() const override { return Component::TCameraComponent; }

	protected:
		void SetViewMatrix(const glm::mat4& view);
	};
}