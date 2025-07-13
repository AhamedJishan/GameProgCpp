#include "Game.h"

namespace jLab
{
	Game::Game()
		:m_IsRunning(true)
	{
	}

	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialize SDL");
			return false;
		}

		// TODO: Init Renderer
		// TODO: Init AudioSystem

		m_TicksCount = SDL_GetTicks();
		LoadData();

		return true;
	}

	void Game::Shutdown()
	{
		UnloadData();

		SDL_Quit();
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
	
	void Game::ProcessInput()
	{
	}
	
	void Game::UpdateGame()
	{
		// Cap FPS at 60
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;

		if (deltaTime > 0.05f)
			deltaTime = 0.05f;

		m_TicksCount = SDL_GetTicks();


	}
	
	void Game::GenerateOutput()
	{
	}
	
	void Game::LoadData()
	{
	}
	
	void Game::UnloadData()
	{
	}
}