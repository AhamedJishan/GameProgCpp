#include "SpriteComponent.h"

#include "Actor.h"
#include "Game.h"

namespace ch3
{
	SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
		:Component(owner),
		m_DrawOrder(drawOrder),
		m_TexWidth(0),
		m_TexHeight(0),
		m_Texture(nullptr)
	{
		m_Owner->getGame()->addSprite(this);
	}
	
	SpriteComponent::~SpriteComponent()
	{
		m_Owner->getGame()->removeSprite(this);
	}
	
	void SpriteComponent::draw(SDL_Renderer* renderer)
	{
		if (m_Texture)
		{
			SDL_Rect r;

			r.w = static_cast<int>(m_TexWidth * m_Owner->getScale());
			r.h = static_cast<int>(m_TexHeight * m_Owner->getScale());

			r.x = static_cast<int>(m_Owner->getPosition().x - r.w / 2);
			r.y = static_cast<int>(m_Owner->getPosition().y - r.h / 2);

			SDL_RenderCopyEx
			(
				renderer,
				m_Texture,
				NULL,
				&r,
				-Math::ToDegrees(m_Owner->getRotation()),
				NULL,
				SDL_FLIP_NONE
			);
		}
	}
	
	void SpriteComponent::setTexture(SDL_Texture* texture)
	{
		m_Texture = texture;
		SDL_QueryTexture(m_Texture, nullptr, nullptr, &m_TexWidth, &m_TexHeight);
	}
}