#include "AnimSpriteComponent.h"

namespace ch3
{
	AnimSpriteComponent::AnimSpriteComponent(Actor* actor, int drawOrder, float fps)
		:SpriteComponent(actor, drawOrder),
		m_CurrFrame(0.0f),
		m_FPS(fps)
	{
	}
	
	void AnimSpriteComponent::update(float deltaTime)
	{
		SpriteComponent::update(deltaTime);

		if (m_AnimTextures.size() > 0)
		{
			while (m_CurrFrame >= m_AnimTextures.size())
				m_CurrFrame -= m_AnimTextures.size();

			SpriteComponent::setTexture( m_AnimTextures[static_cast<int>(m_CurrFrame)] );

			m_CurrFrame += m_FPS * deltaTime;
		}
	}
	
	void AnimSpriteComponent::setAnimTextures(std::vector<SDL_Texture*> textures)
	{
		m_AnimTextures = textures;
		SpriteComponent::setTexture(m_AnimTextures[0]);
		m_CurrFrame = 0.0f;
	}
}