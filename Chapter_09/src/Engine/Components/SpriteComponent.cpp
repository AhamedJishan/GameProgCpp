#include "SpriteComponent.h"

#include <glm/mat4x4.hpp>
#include <GLEW/GL/glew.h>
#include "Engine/Actor.h"
#include "Engine/Texture.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Shader.h"

namespace jLab
{
    SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
        :Component(owner, drawOrder),
        m_Visible(true),
        m_Texture(nullptr),
        m_DrawOrder(drawOrder)
    {
        m_Owner->GetGame()->GetRenderer()->AddSprite(this);
    }
    
    SpriteComponent::~SpriteComponent()
    {
        m_Owner->GetGame()->GetRenderer()->RemoveSprite(this);
    }
    
    void SpriteComponent::Draw(Shader* shader)
    {
        if (m_Texture && m_Visible)
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