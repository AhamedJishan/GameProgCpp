#pragma once

#include "Component.h"
#include <glm/vec3.hpp>

namespace jLab
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(class Actor* owner, int drawOrder = 100);
		~MeshRenderer();

		void Draw(const class Shader* shader);

		void SetMesh(class Model* model) { m_Model = model; }
		void SetSpecularColor(glm::vec3 specularColor, float specularPower) { m_SpecularColor = specularColor; m_SpecularPower = specularPower; }
		void SetVisible(bool visible) { m_IsVisible = visible; }

	private:
		class Model* m_Model;

		glm::vec3 m_SpecularColor;
		float m_SpecularPower;
		bool m_IsVisible;
	};
}