#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace jLab
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(class Actor* owner, int updateOrder = 100);
		~MeshRenderer();

		void Draw(const class Shader* shader);

		void SetModel(class Model* model) { m_Model = model; }
		void SetSpecular(glm::vec3 color, float power) { m_SpecularColor = color; m_SpecularPower = power; }

	private:
		class Model* m_Model;

		glm::vec3 m_SpecularColor;
		float m_SpecularPower;
	};
}