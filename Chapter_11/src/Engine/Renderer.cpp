#include "Renderer.h"

#include <GLEW/GL/glew.h>
#include "Game.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
	{
		m_Game = game;
	}

	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		m_ScreenWidth = screenWidth;
		m_ScreenHeight = screenHeight;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_Window = SDL_CreateWindow("Chapter_11: UI", 128, 72, m_ScreenWidth, m_ScreenHeight, SDL_WINDOW_OPENGL);
		if (m_Window == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create SDL Window: %s",  SDL_GetError());
			return false;
		}

		m_Context = SDL_GL_CreateContext(m_Window);
		if (m_Window == NULL)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create GL Context: %s", SDL_GetError());
			return false;
		}

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLEW!");
			return false;
		}
		glGetError();

		return true;
	}
	
	void Renderer::Shutdown()
	{
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
	}
	
	void Renderer::Draw()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Render scene

		SDL_GL_SwapWindow(m_Window);
	}

	Texture* Renderer::GetTexture(const std::string& filename, Texture::TextureType type)
	{
		auto iter = m_Textures.find(filename);
		if (iter != m_Textures.end())
			return iter->second;

		Texture* texture = new Texture();

		if (!texture->Load(filename, type))
		{
			delete texture;
			texture = nullptr;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture '%s'", filename.c_str());
		}
		else
			m_Textures.emplace(filename, texture);

		return texture;
	}

}