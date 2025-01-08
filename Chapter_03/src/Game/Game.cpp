#include "Game.h"

namespace ch3
{
	Game::Game()
	{
		m_IsRunnning = true;
		m_TicksCount = 0;
	}
	
	bool Game::init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_Window = SDL_CreateWindow("Chapter 03 - Vectors and Basic Physics", 0, 0, 1920, 1080, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
		if (!m_Window)
		{
			SDL_Log("Failed to Create a Window: %s", SDL_GetError());
			return false;
		}

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_Renderer)
		{
			SDL_Log("Failed to Initialise the Renderer: %s", SDL_GetError());
			return false;
		}

		IMG_Init(IMG_INIT_PNG);
		return true;
	}
	
	void Game::runLoop()
	{
		while (m_IsRunnning)
		{
			processInput();
			updateGame();
			generateOutput();
		}
	}
	
	void Game::shutdown()
	{
		IMG_Quit();
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	SDL_Texture* Game::getTexture(const char* filename)
	{
		SDL_Surface* surface = IMG_Load(filename);
		if (!surface)
		{
			SDL_Log("Failed to load the texture at '%s'", filename);
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
		if (!texture)
		{
			SDL_Log("Failed to create the texture from '%s'", filename);
			return nullptr;
		}

		return texture;
	}
	
	void Game::processInput()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_IsRunnning = false;
				break;
			default:
				break;
			}
		}

		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		if (keyboardState[SDL_SCANCODE_ESCAPE])
			m_IsRunnning = false;
	}
	
	void Game::updateGame()
	{
		// DeltaTime calculation
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = deltaTime > 0.5f ? 0.5f : deltaTime;
		m_TicksCount = SDL_GetTicks();
	}

	void Game::generateOutput()
	{
		SDL_SetRenderDrawColor(m_Renderer, 20, 20, 20, 255);
		SDL_RenderClear(m_Renderer);

		// Swap Buffers
		SDL_RenderPresent(m_Renderer);
	}
}