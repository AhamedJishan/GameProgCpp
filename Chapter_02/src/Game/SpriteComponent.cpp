#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

namespace ch2
{
	SpriteComponent::SpriteComponent(Actor* actor, int drawOrder)
		:Component(actor),
		m_Texture(nullptr),
		m_DrawOrder(drawOrder),
		m_TexWidth(0),
		m_TexHeight(0)
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
			SDL_Rect r;
			r.h = static_cast<int>(m_TexHeight * m_Owner->GetScale());
			r.w = static_cast<int>(m_TexWidth * m_Owner->GetScale());
			r.x = static_cast<int>(m_Owner->GetPosition().x - r.w / 2);
			r.y = static_cast<int>(m_Owner->GetPosition().y - r.h / 2);

			SDL_RenderCopyEx(renderer, m_Texture, nullptr, &r, -Math::ToDegrees(m_Owner->GetRotation()), nullptr, SDL_FLIP_NONE);
		}
	}

	void SpriteComponent::SetTexture(SDL_Texture* texture)
	{
		m_Texture = texture;

		SDL_QueryTexture(m_Texture, nullptr, nullptr, &m_TexWidth, &m_TexHeight);
	}
}