#include "SpriteComponent.h"

#include "Shader.h"
#include <GLEW/GL/glew.h>
#include "Game.h"
#include "Actor.h"
#include "util/Math.h"

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
		// TODO: implement sprite based scaling
		// Matrix4 scaleMat = Matrix4::CreateScale(Vector3(m_TexWidth, m_TexHeight, 1));
		// Matrix4 world = scaleMat * m_Owner->GetWorldTranform();

		Matrix4 world = m_Owner->GetWorldTranform();

		shader->SetActive();
		shader->SetMatrixUniform("u_WorldTransform", world);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}