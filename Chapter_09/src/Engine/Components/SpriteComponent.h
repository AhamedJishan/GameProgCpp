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
		void SetTexture(class Texture* texture) { m_Texture = texture; }

		int GetDrawOrder() const { return m_DrawOrder; }

		bool GetVisible() const { return m_Visible; }
		void SetVisible(bool visible) { m_Visible = visible; }

	private:
		class Texture* m_Texture;
		int m_DrawOrder;
		bool m_Visible;
	};
}