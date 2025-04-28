#include "SpriteComponent.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Texture.h"
#include "Engine/Shader.h"
#include "Engine/Actor.h"
#include "Engine/Renderer.h"
#include "Engine/Game.h"


namespace jLab
{
	SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
		:Component(owner, drawOrder),
		m_Texture(nullptr),
		m_DrawOrder(drawOrder)
	{
		m_Owner->GetGame()->GetRenderer()->AddSpriteComponent(this);
	}
	
	SpriteComponent::~SpriteComponent()
	{
		m_Owner->GetGame()->GetRenderer()->RemoveSpriteComponent(this);
	}
	
	void SpriteComponent::Draw(const Shader* shader)
	{
		if (m_Texture)
		{
			glm::mat4 world = glm::mat4(1);
			world = glm::scale(world, glm::vec3(m_Texture->GetWidth(), m_Texture->GetHeight(), 1));
			world = m_Owner->GetWorldTransform() * world;

			shader->SetMat4("u_WorldTransform", world);
			m_Texture->SetActive();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}