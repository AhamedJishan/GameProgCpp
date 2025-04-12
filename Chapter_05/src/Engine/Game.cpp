#include "Game.h"

#include <GLEW/GL/glew.h>
#include <SDL/SDL_image.h>

#include "VertexArray.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Window = nullptr;
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_Window = SDL_CreateWindow("Chapter_05: Opengl", 100, 100, 1280, 720, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_Log("Failed to create Window: %s", SDL_GetError());
			return false;
		}

		m_Context = SDL_GL_CreateContext(m_Window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			SDL_Log("Failed to Initialise GLEW");
			return false;
		}
		glGetError();

		IMG_Init(IMG_INIT_PNG);

		InitSpriteVerts();

		return true;
	}
	
	void Game::Run()
	{
		while (m_IsRunning)
		{
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
	}
	
	void Game::ShutDown()
	{
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
	
	void Game::ProcessInput()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_IsRunning = false;
				break;
			default:
				break;
			}
		}

		const Uint8* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
	}
	
	void Game::UpdateGame()
	{
	}
	
	void Game::GenerateOutput()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: Draw the scene

		SDL_GL_SwapWindow(m_Window);
	}
	
	void Game::InitSpriteVerts()
	{
		float vertices[] = {
			-0.5f,  0.5f, 0.f, // top left
			 0.5f,  0.5f, 0.f, // top right
			 0.5f, -0.5f, 0.f, // bottom right
			-0.5f, -0.5f, 0.f, // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_SpriteVerts = new VertexArray(vertices, 4, indices, 6);
	}

}