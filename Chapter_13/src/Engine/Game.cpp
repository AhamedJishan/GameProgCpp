#include "Game.h"

#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <SDL/SDL.h>

#include "Renderer.h"
#include "InputSystem.h"

namespace jLab
{
	Game::Game()
	{
		mIsRunning = true;
		mUpdatingActors = false;
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

		mInputSystem = new InputSystem(this);
		mInputSystem->Init();

		return true;
	}

	void Game::Shutdown()
	{
		mRenderer->Shutdown();
		delete mRenderer;

		SDL_Quit();
	}

	void Game::AddActor(Actor* actor)
	{
		if (mUpdatingActors)
			mPendingActors.push_back(actor);
		else
			mActors.push_back(actor);
	}

	void Game::RemoveActor(Actor* actor)
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end())
		{
			std::iter_swap(it, mActors.end());
			mActors.pop_back();
		}

		it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		if (it != mPendingActors.end())
		{
			std::iter_swap(it, mPendingActors.end());
			mPendingActors.pop_back();
		}
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
		SDL_Event event;

		mInputSystem->PreUpdate();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				mIsRunning = false;
				break;
			default:
				break;
			}
		}
		mInputSystem->Update();
		InputState inputState = mInputSystem->GetState();

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_ESCAPE))
			mIsRunning = false;
	}

	void Game::UpdateGame()
	{
	}

	void Game::GenerateOutput()
	{
		mRenderer->Draw();
	}
}