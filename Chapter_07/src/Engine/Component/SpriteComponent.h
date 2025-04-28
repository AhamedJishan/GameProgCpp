#pragma once

#include "Component.h"

namespace jLab
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, int drawOrder = 100);
		~SpriteComponent();

		void Draw(const class Shader* shader);

		void SetTexture(class Texture* texture) { m_Texture = texture; }

		int GetDrawOrder() { return m_DrawOrder; }
		class Texture* GetTexture() { return m_Texture; }

	private:
		int m_DrawOrder;
		class Texture* m_Texture;
	};
}