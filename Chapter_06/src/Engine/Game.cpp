#include "Game.h"

#include "GLEW/GL/glew.h"

namespace jLab
{
	Game::Game()
	{
		m_Window = nullptr;
		m_Context = NULL;
		m_IsRunning = true;
		m_TicksCount = 0;
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("ERROR: Failed To Initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_Window = SDL_CreateWindow("Chapter_06: 3D OpenGl", 100, 30, 1280, 720, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_Log("ERROR: Failed to Create Window: %s", SDL_GetError());
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

		m_Context = SDL_GL_CreateContext(m_Window);

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			SDL_Log("ERROR: Failed to Initialise GLEW");
			return false;
		}
		glGetError();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	
	void Game::UpdateGame()
	{
		float deltaTime = (SDL_GetTicks() - m_TicksCount)/1000.0f;
		deltaTime = deltaTime > 0.05 ? 0.05 : deltaTime;
		m_TicksCount = SDL_GetTicks();


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

		const uint8_t* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
	}
	
	void Game::GenerateOutput()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: Render scene

		SDL_GL_SwapWindow(m_Window);
	}
}