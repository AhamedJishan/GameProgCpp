#include "SpriteComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Texture.h"
#include "Engine/Shader.h"
#include <glm/mat4x4.hpp>

namespace jLab
{
	SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
		:Component(owner, drawOrder),
		m_DrawOrder(drawOrder),
		m_Texture(nullptr)
	{
		m_Owner->GetGame()->GetRenderer()->AddSpriteComponent(this);
		InitSpriteVAO();
	}
	
	SpriteComponent::~SpriteComponent()
	{
		m_Owner->GetGame()->GetRenderer()->RemoveSpriteComponent(this);
	}
	
	void SpriteComponent::Draw(Shader* shader)
	{
		if (m_Texture)
		{
			glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(m_Width, m_Height, 1));

			shader->SetMat4("uWorldTransform", m_Owner->GetWorldTransform() * scale);

			glBindVertexArray(m_SpriteVAO);
			m_Texture->SetActive();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	void SpriteComponent::SetTexture(Texture* texture)
	{
		m_Texture = texture;
		m_Width = m_Texture->GetWidth();
		m_Height = m_Texture->GetHeight();
	}
	
	void SpriteComponent::InitSpriteVAO()
	{
		float vertices[] = {
			-0.5f, 0.5f, 0.f,	 0.f, 0.f, 0.0f,	 0.f, 0.f, // top left
			 0.5f, 0.5f, 0.f,	 0.f, 0.f, 0.0f,	 1.f, 0.f, // top right
			 0.5f,-0.5f, 0.f,	 0.f, 0.f, 0.0f,	 1.f, 1.f, // bottom right
			-0.5f,-0.5f, 0.f,	 0.f, 0.f, 0.0f,	 0.f, 1.f  // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &m_SpriteVAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_SpriteVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));

		glBindVertexArray(0);
	}
}