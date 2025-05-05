#include "Game.h"

#include <SDL/SDL.h>
#include <algorithm>
#include "Renderer.h"
#include "InputSystem.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_InputSystem = new InputSystem();
		m_Renderer = new Renderer(this);
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_InputSystem->Init();
		m_Renderer->Init(SCREEN_WIDTH, SCREEN_HEIGHT);

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
		UnloadData();
		m_Renderer->Shutdown();
		delete m_Renderer;
		SDL_Quit();
	}
	
	void Game::ProcessInput()
	{
		m_InputSystem->PreUpdate();

		SDL_Event event;
		while (SDL_PollEvent(&event))
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

		// TODO: Pass the input state to all actors
	}
	
	void Game::UpdateGame()
	{
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = std::min(0.5f, deltaTime);
		m_TicksCount = SDL_GetTicks();
		// FPS lock at around 60fps
		while (SDL_GetTicks() < (m_TicksCount + 16));

		// TODO: update all actors
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}
	
	void Game::LoadData()
	{
	}
	void Game::UnloadData()
	{
	}
}