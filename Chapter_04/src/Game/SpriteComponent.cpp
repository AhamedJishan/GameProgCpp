#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

namespace jLab
{
	SpriteComponent::SpriteComponent(Actor* owner, int updateOrder)
		:Component(owner),
		m_Texture(nullptr),
		m_UpdateOrder(updateOrder)
	{
		m_Owner->GetGame()->AddSprite(this);
	}
	
	SpriteComponent::~SpriteComponent()
	{
		m_Owner->GetGame()->RemoveSprite(this);
	}
	
	void SpriteComponent::Draw(SDL_Renderer* renderer)
	{
		if (m_Texture)
		{
			SDL_Rect rect;
			rect.w = static_cast<int>(m_Owner->GetScale() * m_TexWidth);
			rect.h = static_cast<int>(m_Owner->GetScale() * m_TexHeight);
			rect.x = static_cast<int>(m_Owner->GetPosition().x - rect.w / 2);
			rect.y = static_cast<int>(m_Owner->GetPosition().y - rect.h / 2);

			SDL_RenderCopyEx(renderer, m_Texture, NULL, &rect, -Math::ToDegrees(m_Owner->GetRotation()), NULL, SDL_FLIP_NONE);
		}
	}
	
	void SpriteComponent::SetTexture(SDL_Texture* texture)
	{
		m_Texture = texture;
		SDL_QueryTexture(m_Texture, NULL, NULL, &m_TexWidth, &m_TexHeight);
	}
}