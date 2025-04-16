#include "SpriteComponent.h"

#include "Shader.h"
#include "Texture.h"
#include <GLEW/GL/glew.h>
#include "Game.h"
#include "Actor.h"
#include "util/Math.h"

namespace jLab
{
	SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
		:Component(owner, drawOrder),
		m_Texture(nullptr),
		m_TexHeight(0),
		m_TexWidth(0),
		m_DrawOrder(drawOrder)
	{
		m_Owner->GetGame()->AddSprite(this);
	}
	
	SpriteComponent::~SpriteComponent()
	{
		m_Owner->GetGame()->RemoveSprite(this);
	}
	
	void SpriteComponent::Draw(Shader* shader)
	{
		Matrix4 scaleMat = Matrix4::CreateScale(Vector3(m_TexWidth, m_TexHeight, 1));
		Matrix4 world = scaleMat * m_Owner->GetWorldTranform();

		shader->SetActive();
		shader->SetMatrixUniform("u_WorldTransform", world);

		m_Texture->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	void SpriteComponent::SetTexture(Texture* texture)
	{
		m_Texture = texture;
		m_TexWidth = texture->GetWidth();
		m_TexHeight = texture->GetHeight();
	}
}