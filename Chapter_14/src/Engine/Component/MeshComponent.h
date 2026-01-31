#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent(class Actor* owner, bool isSkinned = false, int updateOrder = 100);
		virtual ~MeshComponent();

		virtual void Draw(const class Shader* shader);

		void SetMesh(class Model* model) { mModel = model; }
		void SetSpecular(glm::vec3 color, float power = 32.0f) { mSpecularColor = color; mSpecularPower = power; }
		void SetVisible(bool value) { mIsVisible = value; }

		bool IsSkinned() { return mIsSkinned; }

		Component::TypeID GetType() const override { return Component::TMeshComponent; }

		// Load/Save
		void LoadProperties(const rapidjson::Value& inObj) override;

	private:
		class Model* mModel;

		glm::vec3 mSpecularColor;
		float mSpecularPower;
		
		bool mIsSkinned;
		bool mIsVisible;
	};
}