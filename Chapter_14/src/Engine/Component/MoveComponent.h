#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class MoveComponent : public Component
	{
	public:
		MoveComponent(class Actor* owner, int updateOrder = 100);

		void Update(float deltaTime) override;
		
		void SetVelocity(glm::vec3 velocity) { mVelocity = velocity; }
		void SetAngularVelocity(float angularVelocity) { mAngularVelocity = angularVelocity; }

		glm::vec3 GetVelocity() const { return mVelocity; }
		float GetAngularVelocity() const { return mAngularVelocity; }

		Component::TypeID GetType() const override { return Component::TMoveComponent; }

	protected:
		glm::vec3 mVelocity;
		float mAngularVelocity;
	};
}