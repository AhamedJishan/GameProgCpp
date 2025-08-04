#include "Game.h"

#include "Renderer.h"
#include "InputSystem.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = SDL_GetTicks();
		m_Renderer = new Renderer(this);
		m_InputSystem = new InputSystem();
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

		m_InputSystem->Init();

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
				m_IsRunning = false;
				break;
			case SDL_MOUSEWHEEL:
				m_InputSystem->ProcessEvent(event);
				break;
			default:
				break;
			}
		}

		m_InputSystem->Update();

		const Uint8* keyBoardState = SDL_GetKeyboardState(NULL);

		if (keyBoardState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
	}
	
	void Game::UpdateGame()
	{
		// Frame cap at 60fps
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));
		// Get deltaTime
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;
		m_TicksCount = SDL_GetTicks();
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}

}