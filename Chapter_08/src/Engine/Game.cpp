#include "Game.h"

#include <SDL/SDL.h>
#include "Input/InputSystem.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_InputSystem = new InputSystem();
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_InputSystem->Init();

		LoadData();

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
		m_InputSystem->Shutdown();
		UnloadData();
		SDL_Quit();
	}
	
	void Game::ProcessInput()
	{
		SDL_Event event;

		m_InputSystem->PreUpdate();

		while (!SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				m_IsRunning = false;
				break;
			}
			default:
				break;
			}
		}

		m_InputSystem->Update();

		// TODO: Pass Input state to all actors
	}
	
	void Game::UpdateGame()
	{
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