#include "Renderer.h"

#include <cstdio>
#include <algorithm>
#include <GLEW/GL/glew.h>

#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Component/MeshComponent.h"
#include "Component/SkinnedMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "UIScreen.h"
#include "GBuffer.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
	{
		mGame = game;
	}

	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		mScreenWidth = screenWidth;
		mScreenHeight = screenHeight;

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		mWindow = SDL_CreateWindow("Chapter_13: Intermediate Graphics", 50, 50, mScreenWidth, mScreenHeight, SDL_WINDOW_OPENGL);
		if (!mWindow)
		{
			printf("ERROR: Failed to Create Window\n\t'%s'\n", SDL_GetError());
			return false;
		}

		mContext = SDL_GL_CreateContext(mWindow);
		if (!mContext)
		{
			printf("ERROR: Failed to create Gl Context\n\t'%s'\n", SDL_GetError());
			return false;
		}

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			printf("ERROR: Failed to Initialize GLEW\n");
			return false;
		}
		glGetError();

		mGBuffer = new GBuffer();
		if (!mGBuffer->Create(mScreenWidth, mScreenHeight))
		{
			printf("ERROR: Failed to create GBuffer!\n");
			delete mGBuffer;
			return false;
		}

		if (!LoadShaders())
		{
			printf("ERROR: Failed to Load Shaders!\n");
			return false;
		}

		InitSpriteQuad();
		CreateMirrorRenderTarget();

		return true;
	}

	void Renderer::Shutdown()
	{
		delete mGBuffer;
		DeleteSpriteQuad();
		DeleteMirrorRenderTarget();
		SDL_GL_DeleteContext(mContext);
		SDL_DestroyWindow(mWindow);
	}

	void Renderer::Draw()
	{
		// 3D Render Pass
		Draw3DScene(mMirrorFBO, mMirrorView, 0.25f, true);
		Draw3DScene(mGBuffer->GetId(), mView, 1.0f, false);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		DrawFromGBuffer();

		// 2D Render Pass
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
		UseSpriteQuad();
		for (SpriteComponent* sprite : mSprites)
			sprite->Draw(mSpriteShader);
		for (UIScreen* screen : mGame->GetUIStack())
			screen->Draw(mSpriteShader);
		glDisable(GL_BLEND);

		SDL_GL_SwapWindow(mWindow);
	}

	void Renderer::AddMeshComponent(MeshComponent* mesh)
	{
		if (mesh->IsSkinned())
			mSkinnedMeshes.emplace_back(static_cast<SkinnedMeshComponent*>(mesh));
		else
			mMeshes.emplace_back(mesh);
	}

	void Renderer::RemoveMeshComponent(MeshComponent* mesh)
	{
		auto it = std::find(mMeshes.begin(), mMeshes.end(), mesh);
		if (it != mMeshes.end())
			mMeshes.erase(it);

		auto it2 = std::find(mSkinnedMeshes.begin(), mSkinnedMeshes.end(), static_cast<SkinnedMeshComponent*>(mesh));
		if (it2 != mSkinnedMeshes.end())
			mSkinnedMeshes.erase(it2);
	}

	void Renderer::AddSpriteComponent(SpriteComponent* sprite)
	{
		int drawOrder = sprite->GetDrawOrder();
		auto it = mSprites.begin();
		for (; it != mSprites.end(); it++)
			if (drawOrder < (*it)->GetDrawOrder())
				break;
		mSprites.insert(it, sprite);
	}

	void Renderer::RemoveSpriteComponent(SpriteComponent* sprite)
	{
		auto it = std::find(mSprites.begin(), mSprites.end(), sprite);
		if (it != mSprites.end())
			mSprites.erase(it);
	}

	Texture* Renderer::GetTexture(const std::string& filename, bool flipVertically, Texture::Type type)
	{
		auto iter = mTextures.find(filename);
		if (iter != mTextures.end())
			return iter->second;

		Texture* texture = new Texture();

		if (!texture->Load(filename, flipVertically, type))
		{
			delete texture;
			texture = nullptr;
			printf("Failed to load texture '%s'", filename.c_str());
		}
		else
			mTextures.emplace(filename, texture);

		return texture;
	}
	Texture* Renderer::GetTexture(const std::string& filename, const aiTexture* textureData, bool flipVertically, Texture::Type type)
	{
		auto iter = mTextures.find(filename);
		if (iter != mTextures.end())
			return iter->second;

		Texture* texture = new Texture();

		if (!texture->Load(filename, textureData, flipVertically, type))
		{
			delete texture;
			texture = nullptr;
			printf("Failed to load texture '%s'", filename.c_str());
		}
		else
			mTextures.emplace(filename, texture);

		return texture;
	}

	Model* Renderer::GetModel(const std::string& filename, bool flipUVs, Skeleton* skeleton)
	{
		auto it = mModels.find(filename);
		if (it != mModels.end())
			return it->second;

		Model* model = new Model(filename, mGame, flipUVs, skeleton);
		mModels.emplace(filename, model);

		return model;
	}

	void Renderer::SetShaderUniforms(const Shader* shader, const glm::mat4& view)
	{
		glm::mat3 R = glm::mat3(mView);
		glm::vec3 T = glm::vec3(mView[3]);
		glm::vec3 cameraPos = -glm::transpose(R) * T;

		shader->SetVec3("uCameraPos", cameraPos);
		shader->SetVec3("uLightDir", glm::vec3(1, -0.5f, -1));
		shader->SetVec3("uLightColor", glm::vec3(1));
		shader->SetVec3("uAmbientColor", glm::vec3(0.2f));
	}

	void Renderer::InitSpriteQuad()
	{
		float vertices[] = {
			-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
			 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
			 0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
			-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &mSpriteVAO);
		glGenBuffers(1, &mSpriteVBO);
		glGenBuffers(1, &mSpriteEBO);

		glBindVertexArray(mSpriteVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mSpriteVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSpriteEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Renderer::DeleteSpriteQuad()
	{
		glDeleteVertexArrays(1, &mSpriteVAO);
		glDeleteBuffers(1, &mSpriteVBO);
		glDeleteBuffers(1, &mSpriteEBO);
	}

	void Renderer::UseSpriteQuad()
	{
		glBindVertexArray(mSpriteVAO);
		mSpriteShader->SetActive();
		mSpriteShader->SetMat4("uViewProjection", mOrtho);
	}

	bool Renderer::LoadShaders()
	{
		mProjection = glm::perspective(glm::radians(80.0f), (float)(mScreenWidth) / (float)(mScreenHeight), 0.1f, 1000.0f);
		mView = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		mOrtho = glm::ortho(-mScreenWidth / 2.0f, mScreenWidth / 2.0f, -mScreenHeight / 2.0f, mScreenHeight / 2.0f);
		mMirrorView = mView;

		mMeshShader = new Shader();
		if (!mMeshShader->Load("Assets/Shaders/Phong.vert", "Assets/Shaders/GBufferWrite.frag"))
			return false;

		mSkinnedMeshShader = new Shader();
		if (!mSkinnedMeshShader->Load("Assets/Shaders/Skinned.vert", "Assets/Shaders/GBufferWrite.frag"))
			return false;

		mGGlobalShader = new Shader();
		if (!mGGlobalShader->Load("Assets/Shaders/GBufferGlobal.vert", "Assets/Shaders/GBufferGlobal.frag"))
			return false;
		mGGlobalShader->SetActive();
		mGGlobalShader->SetMat4("uViewProjection", mOrtho);
		mGGlobalShader->SetMat4("uWorldTransform", glm::scale(glm::mat4(1), glm::vec3(mScreenWidth, -mScreenHeight, 0)));
		mGGlobalShader->SetInt("uGDiffuse", static_cast<int>(GBuffer::Type::Diffuse));
		mGGlobalShader->SetInt("uGNormal", static_cast<int>(GBuffer::Type::Normal));
		mGGlobalShader->SetInt("uGWorldPos", static_cast<int>(GBuffer::Type::WorldPosition));

		mSpriteShader = new Shader();
		if (!mSpriteShader->Load("Assets/Shaders/Sprite.vert", "Assets/Shaders/Sprite.frag"))
			return false;
		mSpriteShader->SetActive();
		mSpriteShader->SetMat4("uViewProjection", mProjection * mView);

		return true;
	}

	bool Renderer::CreateMirrorRenderTarget()
	{
		int width = static_cast<int>(mScreenWidth / 4.0f);
		int height = static_cast<int>(mScreenHeight / 4.0f);

		glGenFramebuffers(1, &mMirrorFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mMirrorFBO);

		mMirrorTexture = new Texture();
		mMirrorTexture->CreateForRendering(width, height, GL_RGB);

		unsigned int depthRBO;
		glGenRenderbuffers(1, &depthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mMirrorTexture->GetID(), 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

		GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			glDeleteFramebuffers(1, &mMirrorFBO);
			delete mMirrorTexture;
			mMirrorTexture = nullptr;
			return false;
		}

		return true;
	}

	void Renderer::DeleteMirrorRenderTarget()
	{
		glDeleteFramebuffers(1, &mMirrorFBO);
		delete mMirrorTexture;
		mMirrorTexture = nullptr;
	}

	void Renderer::Draw3DScene(unsigned int framebuffer, const glm::mat4& view, float viewPortScale, bool lit)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glViewport(0, 0, mScreenWidth * viewPortScale, mScreenHeight * viewPortScale);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw 3D Stuff
		glEnable(GL_DEPTH_TEST);

		mMeshShader->SetActive();
		if (lit)
			SetShaderUniforms(mMeshShader, view);
		mMeshShader->SetMat4("uViewProjection", mProjection * view);
		for (MeshComponent* mesh : mMeshes)
			mesh->Draw(mMeshShader);

		mSkinnedMeshShader->SetActive();
		if (lit)
			SetShaderUniforms(mSkinnedMeshShader, view);
		mSkinnedMeshShader->SetMat4("uViewProjection", mProjection * view);
		for (SkinnedMeshComponent* mesh : mSkinnedMeshes)
			mesh->Draw(mSkinnedMeshShader);

		glDisable(GL_DEPTH_TEST);
	}

	void Renderer::DrawFromGBuffer()
	{
		mGGlobalShader->SetActive();
		mGBuffer->SetTexturesActive();

		SetShaderUniforms(mGGlobalShader, mView);

		glBindVertexArray(mSpriteVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	glm::vec3 Renderer::ScreenToWorldPos(const glm::vec3& screenPosition)
	{
		glm::vec3 ndc = screenPosition;
		ndc.x /= mScreenWidth * 0.5f;
		ndc.y /= mScreenHeight * 0.5f;

		glm::mat4 unprojection = glm::inverse(mProjection * mView);
		glm::vec4 pos = unprojection * glm::vec4(ndc, 1.0f);
		pos /= pos.w;

		return glm::vec3(pos);
	}

	void Renderer::ScreenToWorldDir(glm::vec3& outStart, glm::vec3& outDirection)
	{
		glm::vec3 ndcPos = glm::vec3(0);
		outStart = ScreenToWorldPos(ndcPos);
		ndcPos.z = 0.9f;
		glm::vec3 end = ScreenToWorldPos(ndcPos);
		outDirection = glm::normalize(end - outStart);
	}
}