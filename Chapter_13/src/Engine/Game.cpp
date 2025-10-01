#include "Game.h"

#include <cstdio>
#include <SDL/SDL.h>

#include "Renderer.h"

namespace jLab
{
	Game::Game()
	{
		mIsRunning = true;
	}

	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			printf("ERROR: Failed to Initialize SDL\n\t'%s'\n", SDL_GetError());
			return false;
		}

		mRenderer = new Renderer(this);
		if (!mRenderer->Init(1080, 720))
		{
			printf("ERROR: Failed to Initialize Renderer\n");
			return false;
		}

		return true;
	}

	void Game::Shutdown()
	{
	}

	void Game::Run()
	{
		while (mIsRunning)
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
	}

	void Game::GenerateOutput()
	{
		mRenderer->Draw();
	}
}