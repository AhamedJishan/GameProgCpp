#include "Game.h"

#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <SDL/SDL.h>

#include "Renderer.h"
#include "InputSystem.h"
#include "PhysWorld.h"
#include "Actor.h";
#include "Component/MeshComponent.h"
#include "Model.h"

namespace jLab
{
	Game::Game()
	{
		mTicksCount = 0;
		mUpdatingActors = false;
		mGameState = GameState::Gameplay;
	}

	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			printf("ERROR: Failed to Initialize SDL\n\t'%s'\n", SDL_GetError());
			return false;
		}

		mRenderer = new Renderer(this);
		if (!mRenderer->Init(1280, 720))
		{
			printf("ERROR: Failed to Initialize Renderer\n");
			return false;
		}

		mInputSystem = new InputSystem(this);
		mInputSystem->Init();
		mPhysWorld = new PhysWorld(this);

		LoadData();

		return true;
	}

	void Game::Shutdown()
	{
		UnloadData();
		mRenderer->Shutdown();
		delete mRenderer;
		mInputSystem->Shutdown();
		delete mInputSystem;

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
		while (mGameState != GameState::Quit)
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
				mGameState = GameState::Quit;
				break;
			default:
				break;
			}
		}
		mInputSystem->Update();
		InputState inputState = mInputSystem->GetState();

		if (inputState.Keyboard.GetKey(SDL_SCANCODE_ESCAPE))
			mGameState = GameState::Quit;

		if (mGameState == GameState::Gameplay)
		{
			mUpdatingActors = true;
			for (Actor* actor : mActors)
				actor->ProcessInput(inputState);
			mUpdatingActors = false;
		}
	}

	void Game::UpdateGame()
	{
		float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
		if (deltaTime > 0.05f)
			deltaTime = 0.05f;
		mTicksCount = SDL_GetTicks();

		if (mGameState == GameState::Gameplay)
		{
			// Update actors
			mUpdatingActors = true;
			for (Actor* actor : mActors)
				actor->ProcessUpdate(deltaTime);
			mUpdatingActors = false;

			// Move Pending Actors to Actors
			for (Actor* actor : mPendingActors)
				mActors.emplace_back(actor);
			mPendingActors.clear();

			// Move dead actors to temp list
			std::vector<Actor*> deadActors;
			for (Actor* actor : mActors)
				if(actor->GetState() == Actor::State::Dead) deadActors.emplace_back(actor);

			// Delete dead Actors
			for (Actor* actor : deadActors)
				delete actor;
			deadActors.clear();
		}
	}

	void Game::GenerateOutput()
	{
		mRenderer->Draw();
	}

	void Game::LoadData()
	{
		Actor* testModel = new Actor(this);
		testModel->SetScale(glm::vec3(0.01f));
		testModel->SetPosition(glm::vec3(0, -1, -1.5f));

		MeshComponent* mc = new MeshComponent(testModel);
		mc->SetMesh(mRenderer->GetModel("Assets/Models/eve/eve.dae", false));
	}
	
	void Game::UnloadData()
	{
	}
}