#pragma once

#include "Component.h"
#include <SDL.h>

namespace ch2
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* actor, int drawOrder);
		~SpriteComponent();

		virtual void Draw (SDL_Renderer* renderer);
		virtual void SetTexture(SDL_Texture* texture);

		int GetDrawOrder() const { return m_DrawOrder; }
		int GetTexWidth() const { return m_TexWidth; }
		int GetTexHeight() const { return m_TexHeight; }

	protected:
		SDL_Texture* m_Texture;

		int m_DrawOrder;
		int m_TexWidth;
		int m_TexHeight;
	};
}