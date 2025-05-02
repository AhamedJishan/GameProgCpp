#include "Game.h"

#include <SDL/SDL.h>
#include "InputSystem.h"
#include "Renderer.h"

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

		m_Renderer->Init(1280, 720);
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
		m_Renderer->Shutdown();
		m_InputSystem->Shutdown();
		UnloadData();
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
			case SDL_KEYDOWN:
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
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
		if (m_InputSystem->GetInputState().Keyboard.GetKeyState(SDL_SCANCODE_SPACE) == ButtonState::E_Released)
			SDL_Log("Space Released");
		if (m_InputSystem->GetInputState().Keyboard.GetKeyState(SDL_SCANCODE_SPACE) == ButtonState::E_Pressed)
			SDL_Log("Space Pressed");
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