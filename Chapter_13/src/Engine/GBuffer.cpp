#include "GBuffer.h"

#include <GLEW/GL/glew.h>
#include "Texture.h"

namespace jLab
{
	GBuffer::GBuffer()
		:mId(0)
	{
	}

	GBuffer::~GBuffer()
	{
		Destroy();
	}
	
	bool GBuffer::Create(int width, int height)
	{
		glGenFramebuffers(1, &mId);
		glBindFramebuffer(GL_FRAMEBUFFER, mId);

		unsigned int depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		std::vector<GLenum> attachments;
		for (int i = 0; i < static_cast<int>(Type::NUM_GBUFFER_TEXTURES); i++)
		{
			Texture* texture = new Texture();
			texture->CreateForRendering(width, height, GL_RGB32F);
			mTextures.emplace_back(texture);

			attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, texture->GetID(), 0);
		}

		glDrawBuffers(attachments.size(), attachments.data());

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			Destroy();
			return false;
		}

		return true;
	}
	
	void GBuffer::Destroy()
	{
		glDeleteFramebuffers(1, &mId);
		for (int i = 0; i < mTextures.size(); i++)
			delete mTextures[i];
		mTextures.clear();
	}
	
	Texture* GBuffer::GetTexture(Type type)
	{
		Texture* texture = nullptr;

		if (static_cast<int>(type) < mTextures.size())
			texture = mTextures[static_cast<int>(type)];

		return texture;
	}
	
	void GBuffer::SetTexturesActive()
	{
		for (int i = 0; i < mTextures.size(); i++)
			mTextures[i]->SetActive(i);
	}
}