#include "AnimSpriteComponent.h"

namespace ch2
{
	AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
		:SpriteComponent(owner, drawOrder)
	{
	}

	void AnimSpriteComponent::Update(float deltaTime)
	{
		SpriteComponent::Update(deltaTime);

		if (m_AnimTextures.size() > 0)
		{
			m_CurrFrame = m_AnimFPS * deltaTime;

			while (m_CurrFrame >= m_AnimTextures.size())
				m_CurrFrame -= m_AnimTextures.size();

			SpriteComponent::SetTexture(m_AnimTextures[static_cast<int>(m_CurrFrame)]);
		}
	}

	void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
	{
		m_AnimTextures = textures;
		SpriteComponent::SetTexture(m_AnimTextures[0]);
		m_CurrFrame = 0;
	}
}