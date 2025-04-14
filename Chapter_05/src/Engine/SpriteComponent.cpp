#include "SpriteComponent.h"

#include "Shader.h"
#include <GLEW/GL/glew.h>
#include "Game.h"
#include "Actor.h"

namespace jLab
{
	SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
		:Component(owner, drawOrder),
		m_TexHeight(0),
		m_TexWidth(0)
	{
		m_Owner->GetGame()->AddSprite(this);
	}
	
	SpriteComponent::~SpriteComponent()
	{
		m_Owner->GetGame()->RemoveSprite(this);
	}
	
	void SpriteComponent::Draw(Shader* shader)
	{
		shader->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}