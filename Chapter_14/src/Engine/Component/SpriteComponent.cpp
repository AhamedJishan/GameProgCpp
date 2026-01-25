#include "SpriteComponent.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <GLEW/GL/glew.h>
#include "Engine/Shader.h"
#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"

namespace jLab
{
	SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
		:Component(owner, drawOrder)
		,mWidth(0)
		,mHeight(0)
		,mVisible(true)
		,mTexture(nullptr)
	{
		mOwner->GetGame()->GetRenderer()->AddSpriteComponent(this);
	}

	SpriteComponent::~SpriteComponent()
	{
		mOwner->GetGame()->GetRenderer()->RemoveSpriteComponent(this);
	}
	
	void SpriteComponent::Draw(const Shader* shader)
	{
		if (mTexture && mVisible)
		{
			glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(mWidth, mHeight, 0));
			shader->SetMat4("uWorldTransform", mOwner->GetWorldTransformMatrix() * scaleMat);
			mTexture->SetActive();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
	
	void SpriteComponent::SetTexture(Texture* texture)
	{
		mTexture = texture;
		mWidth = mTexture->GetWidth();
		mHeight = mTexture->GetHeight();
	}
}