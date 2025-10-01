#include "Game.h"

#include <iostream>

namespace jLab
{
	Game::Game()
	{
		mIsRunning = true;
	}

	bool Game::Init()
	{
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
		printf("Generating Game output\n");
	}
}