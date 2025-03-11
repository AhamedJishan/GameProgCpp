#pragma once

#include "Component.h"
#include <SDL/SDL.h>

namespace jLab
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, int updateOrder = 100);
		~SpriteComponent();

		void Draw(SDL_Renderer* renderer);
		void SetTexture(SDL_Texture* texture);

		int GetUpdateOrder() { return m_UpdateOrder; }
		int GetTexWidth() { return m_TexWidth; }
		int GetTexHeight() { return m_TexHeight; }

	private:
		SDL_Texture* m_Texture;
		int m_UpdateOrder;
		int m_TexWidth;
		int m_TexHeight;
	};
}