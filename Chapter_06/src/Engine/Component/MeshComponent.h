#pragma once

#include "Component.h"

namespace jLab
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent(class Actor* owner, int updateOrder = 100);
		~MeshComponent();

		void Draw(const class Shader* shader);

		void SetModel(class Model* model) { m_Model = model; }

	private:
		class Model* m_Model;
	};
}