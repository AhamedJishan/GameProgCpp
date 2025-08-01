#include "Game.h"

#include "Renderer.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = SDL_GetTicks();
		m_Renderer = new Renderer(this);
	}
	
	Game::~Game()
	{
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise SDL");
			return false;
		}

		if (!m_Renderer->Init(1280, 720))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise Renderer");
			delete m_Renderer;
			m_Renderer = nullptr;
			return false;
		}

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
	
	void Game::Shutdown()
	{
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

		const Uint8* keyBoardState = SDL_GetKeyboardState(NULL);

		if (keyBoardState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
	}
	
	void Game::UpdateGame()
	{
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}

}