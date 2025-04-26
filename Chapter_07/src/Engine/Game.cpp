#include "Game.h"

#include <SDL/SDL.h>
#include "Renderer.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Renderer = nullptr;
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialise SDL : % s", SDL_GetError());
			return false;
		}

		m_Renderer = new Renderer(this);
		m_Renderer->Init(1280, 720);

		// TODO: Initialise AudioSystem


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

		const uint8_t* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
		
		// TODO: Call processInput for all actors
	}
	
	void Game::UpdateGame()
	{
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}
}