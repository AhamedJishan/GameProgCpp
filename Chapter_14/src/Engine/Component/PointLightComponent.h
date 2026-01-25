#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class PointLightComponent : public Component
	{
	public:
		PointLightComponent(class Actor* owner);
		~PointLightComponent();

		void Draw(const class Shader* shader);

		glm::vec3 GetDiffuseColor() { return mDiffuseColor; }
		float GetInnerRadius() { return mInnerRadius; }
		float GetOuterRadius() { return mOuterRadius; }

		void SetDiffuseColor(glm::vec3 color) { mDiffuseColor = color; }
		void SetInnerRadius(float value) { mInnerRadius = value; }
		void SetOuterRadius(float value) { mOuterRadius = value; }

	private:
		glm::vec3 mDiffuseColor;
		float mInnerRadius;
		float mOuterRadius;
	};
}