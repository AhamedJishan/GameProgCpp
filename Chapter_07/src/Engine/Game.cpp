#include "Game.h"

#include <SDL/SDL.h>
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Renderer = nullptr;
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialise SDL : % s", SDL_GetError());
			return false;
		}

		m_Renderer = new Renderer(this);
		m_Renderer->Init(1280, 720);

		// TODO: Initialise AudioSystem


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
		SDL_Quit();
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

		const uint8_t* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
		
		for (Actor* actor : m_PendingActors)
			actor->ProcessInput(keyState);
	}
	
	void Game::UpdateGame()
	{
		// DeltaTime Calculations
		float deltaTime = (SDL_GetTicks() - m_TicksCount)/1000.0f;
		deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;
		m_TicksCount = SDL_GetTicks();

		// Lock FPS at 60
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));

		// UPDATE all actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActors = false;

		// MOVE pending actors to m_Actors
		for (Actor* actor : m_PendingActors)
			m_PendingActors.emplace_back(actor);
		m_PendingActors.clear();

		// DELETE dead actors
		std::vector<Actor*> actorsToBeDeleted;

		for (Actor* actor : m_Actors)
			if (actor->GetState() == Actor::E_Dead)
				actorsToBeDeleted.emplace_back(actor);
		// Destructor of the actor calls RemoveActor()
		for (Actor* actor : actorsToBeDeleted)
			delete actor;

		actorsToBeDeleted.clear();
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}



	void Game::AddActor(Actor* actor)
	{
		if (m_UpdatingActors)
			m_PendingActors.emplace_back(actor);
		else
			m_Actors.emplace_back(actor);
	}

	void Game::RemoveActor(Actor* actor)
	{
		auto iter = std::find(m_Actors.begin(), m_Actors.end(), actor);
		if (iter != m_Actors.end())
		{
			std::iter_swap(iter, m_Actors.end() - 1);
			m_Actors.erase(m_Actors.end() - 1);
			return;
		}

		iter = std::find(m_PendingActors.begin(), m_PendingActors.end(), actor);
		if (iter != m_PendingActors.end())
		{
			std::iter_swap(iter, m_PendingActors.end() - 1);
			m_PendingActors.erase(m_PendingActors.end() - 1);
		}
	}
}