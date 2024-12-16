#include "Game.h"

namespace ch2
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
	}
	Game::~Game()
	{
	}

	bool Game::Init()
	{
		int sdlResult = SDL_Init(SDL_INIT_VIDEO);
		if (sdlResult != 0)
		{
			SDL_Log("Failed to Initialise SDL!");
			return false;
		}

		m_Window = SDL_CreateWindow("Chapter_02", 100, 100, 1280, 720, 0);
		if (!m_Window)
		{
			SDL_Log("Failed to create SDL Window!");
			return false;
		}

		return true;
	}

	void Game::RunLoop()
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
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	void Game::ProcessInput()
	{
	}

	void Game::UpdateGame()
	{
	}

	void Game::GenerateOutput()
	{
	}
}