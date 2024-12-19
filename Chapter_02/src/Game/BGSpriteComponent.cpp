#include "BGSpriteComponent.h"
#include "Actor.h"

namespace ch2
{
	BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
		:SpriteComponent(owner, drawOrder)
	{
	}

	void BGSpriteComponent::Update(float deltaTime)
	{
		SpriteComponent::Update(deltaTime);

		for (auto& bgTexture : m_BGTextures)
		{
			bgTexture.m_Offset.x += m_ScrollSpeed * deltaTime;

			if (bgTexture.m_Offset.x < -m_ScreenSize.x)
				bgTexture.m_Offset.x = (m_BGTextures.size() - 1) * m_ScreenSize.x - 1;
		}
	}
	
	void BGSpriteComponent::Draw(SDL_Renderer* renderer)
	{
		if (m_BGTextures.size() > 0)
		{
			SDL_Rect r;
			for (auto& bgTexture : m_BGTextures)
			{
				r.w = static_cast<int>(m_ScreenSize.x);
				r.h = static_cast<int>(m_ScreenSize.y);
				r.x = static_cast<int>(m_Owner->GetPosition().x + bgTexture.m_Offset.x);
				r.y = static_cast<int>(m_Owner->GetPosition().y + bgTexture.m_Offset.y);
				SDL_RenderCopy(renderer, bgTexture.m_Texture, nullptr, &r);
			}
		}
	}
	
	// This function assumes that each background image has a width corresponding to the screen width
	void BGSpriteComponent::SetBGTexture(const std::vector<SDL_Texture*> textures)
	{
		int count = 0;

		for (auto texture : textures)
		{
			BGTexture temp;

			temp.m_Texture = texture;
			temp.m_Offset.x = m_ScreenSize.x * count;
			temp.m_Offset.y = 0;

			m_BGTextures.emplace_back(temp);

			count++;
		}
	}
}