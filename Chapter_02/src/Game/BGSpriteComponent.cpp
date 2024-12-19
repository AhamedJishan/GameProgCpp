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
				r.x = m_Owner->GetPosition().x + bgTexture.m_Offset.x;
				r.y = m_Owner->GetPosition().y + bgTexture.m_Offset.y;
				r.w = m_Owner->GetScale() * m_ScreenSize.x;
				r.h = m_Owner->GetScale() * m_ScreenSize.y;
				SDL_RenderCopyEx(renderer, bgTexture.m_Texture, NULL, &r, -Math::ToDegrees(m_Owner->GetRotation()), NULL, SDL_FLIP_NONE);
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