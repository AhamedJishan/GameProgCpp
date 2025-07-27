#include "SpriteRenderer.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Texture.h"
#include "Engine/Shader.h"
#include <glm/mat4x4.hpp>

namespace jLab
{
	SpriteRenderer::SpriteRenderer(Actor* owner, int drawOrder)
		:Component(owner, drawOrder)
		,m_DrawOrder(drawOrder)
		,m_Height(0)
		,m_Width(0)
		,m_Texture(nullptr)
		,m_Visible(true)
	{
		m_Owner->GetGame()->GetRenderer()->AddSpriteRenderer(this);
	}

	SpriteRenderer::~SpriteRenderer()
	{
		m_Owner->GetGame()->GetRenderer()->RemoveSpriteRenderer(this);
	}
	
	void SpriteRenderer::Draw(const Shader* shader)
	{
		if (m_Texture && m_Visible)
		{
			glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(m_Width, m_Height, 1));

			shader->SetMat4("u_WorldTransform", m_Owner->GetWorldTransform() * scaleMat);
			m_Texture->SetActive();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
	
	void SpriteRenderer::SetTexture(Texture* texture)
	{
		m_Texture = texture;
		m_Width = texture->GetWidth();
		m_Height = texture->GetHeight();
	}
}