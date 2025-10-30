#include "Renderer.h"

#include <cstdio>
#include <algorithm>
#include <GLEW/GL/glew.h>

#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Component/MeshComponent.h"

// TODO: ScreenToWorldPos, ScreenToWorldDir

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

		mMeshShader = new Shader("Assets/Shaders/Phong.vert", "Assets/Shaders/Phong.frag");
		
		mProjection = glm::perspective(glm::radians(80.0f), (float)(mScreenWidth) / (float)(mScreenHeight), 0.1f, 1000.0f);
		mView = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		mOrtho = glm::ortho(-mScreenWidth / 2.0f, mScreenWidth / 2.0f, -mScreenHeight / 2.0f, mScreenHeight / 2.0f);

		return true;
	}

	void Renderer::Shutdown()
	{
		SDL_GL_DeleteContext(mContext);
		SDL_DestroyWindow(mWindow);
	}

	void Renderer::Draw()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		SetShaderUniforms(mMeshShader);
		for (MeshComponent* mesh : mMeshes)
			mesh->Draw(mMeshShader);

		glDisable(GL_DEPTH_TEST);

		SDL_GL_SwapWindow(mWindow);
	}

	void Renderer::AddMeshComponent(MeshComponent* mesh)
	{
		mMeshes.emplace_back(mesh);
	}

	void Renderer::RemoveMeshComponent(MeshComponent* mesh)
	{
		auto it = std::find(mMeshes.begin(), mMeshes.end(), mesh);
		if (it != mMeshes.end())
			mMeshes.erase(it);
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

	void Renderer::SetShaderUniforms(const Shader* shader)
	{
		glm::mat4 viewProjection = mProjection * mView;

		glm::mat3 R = glm::mat3(mView);
		glm::vec3 T = glm::vec3(mView[3]);
		glm::vec3 cameraPos = -glm::transpose(R) * T;

		shader->SetActive();
		shader->SetMat4("uViewProjection", viewProjection);
		shader->SetVec3("uCameraPos", cameraPos);
		shader->SetVec3("uLightDir", glm::vec3(1, -0.5f, -1));
		shader->SetVec3("uLightColor", glm::vec3(1));
		shader->SetVec3("uAmbientColor", glm::vec3(0.3f));
	}
}