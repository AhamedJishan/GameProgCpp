#include "Game.h"

#include <SDL/SDL.h>

namespace jLab
{
	Game::Game()
	{
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

		return true;
	}
	
	void Game::Run()
	{
	}
	
	void Game::Shutdown()
	{
	}
}