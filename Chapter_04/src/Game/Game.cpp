#include "Game.h"

#include <SDL/SDL_image.h>

namespace jLab
{
	Game::Game()
		:m_Renderer(nullptr),
		m_TicksCount(0),
		m_Window(nullptr),
		m_IsRunning(false)
	{
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Unable to initialise SDL '%s'", SDL_GetError());
			return false;
		}
		m_Window = SDL_CreateWindow("Chapter_04", 100, 100, 1280, 720, 0);
		if (!m_Window)
		{
			SDL_Log("Unable to create Window '%s'", SDL_GetError());
			return false;
		}
		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		if (!m_Renderer)
		{
			SDL_Log("Unable to create Renderer '%s'", SDL_GetError());
			return false;
		}
		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			SDL_Log("Unable to initialise SDL_Image '%s'", SDL_GetError());
			return false;
		}

		LoadData();
		m_IsRunning = true;
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
		UnloadData();
		IMG_Quit();
		SDL_DestroyRenderer(m_Renderer);
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
	
	void Game::LoadData()
	{
	}
	
	void Game::UnloadData()
	{
	}
}