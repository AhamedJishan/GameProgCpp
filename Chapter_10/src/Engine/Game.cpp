#include "Game.h"

#include "Renderer.h"

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

		m_Renderer = new Renderer(this);
		if (!m_Renderer->Init(1280, 720))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize Renderer");
			delete m_Renderer;
			m_Renderer = nullptr;
			return false;
		}

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

		const Uint8* state = SDL_GetKeyboardState(NULL);

		if (state[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
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
		m_Renderer->Draw();
	}
	
	void Game::LoadData()
	{
	}
	
	void Game::UnloadData()
	{
	}
}