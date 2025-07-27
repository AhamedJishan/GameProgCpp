#pragma once

#include "Component.h"

namespace jLab
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer(class Actor* owner, int drawOrder = 100);
		~SpriteRenderer();

		void Draw(const class Shader* shader);
		void SetTexture(class Texture* texture);

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		int GetDrawOrder() const { return m_DrawOrder; }

		void SetVisible(bool visible) { m_Visible = visible; }
		bool GetVisible() const { return m_Visible; }

	private:
		class Texture* m_Texture;
		int m_Width;
		int m_Height;
		int m_DrawOrder;
		bool m_Visible;
	};
}