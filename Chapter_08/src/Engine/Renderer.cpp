#include "Renderer.h"

#include "Game.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
	{
		m_Game = game;
		m_Window = nullptr;
		m_Context = nullptr;
	}
	
	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		m_Width = screenWidth;
		m_Height = screenHeight;

		m_Window = SDL_CreateWindow("Chapter_08: Input System", 100, 30, m_Width, m_Height, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_Context = SDL_GL_CreateContext(m_Window);
		if (!m_Context)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create context: %s", SDL_GetError());
			return false;
		}

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise GLEW");
			return false;
		}
		glGetError();

		glViewport(0, 0, m_Width, m_Height);

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
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the scene

		SDL_GL_SwapWindow(m_Window);
	}
}