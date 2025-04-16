#pragma once

#include "Component.h"

namespace jLab
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, int drawOrder = 100);
		~SpriteComponent();

		void Draw(class Shader* shader);

		void SetTexture(class Texture* texture);
		int GetDrawOrder() { return m_DrawOrder; }
		int GetTexWidth() { return m_TexWidth; }
		int GetTexHeight() { return m_TexHeight; }
	private:
		class Texture* m_Texture;
		int m_DrawOrder;
		int m_TexWidth;
		int m_TexHeight;
	};
}