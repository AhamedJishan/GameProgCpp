#include "Game.h"

#include <GLEW/GL/glew.h>
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"
#include "Camera.h"

#include "Game/TestActor.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Renderer = new Renderer(this);
		m_Camera = new Camera(this, 1280, 720, 0.1f, 100.0f, 45.0f);
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("ERROR: Failed To Initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_Renderer->Init(1280, 720);

		LoadData();

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
	
	void Game::ShutDown()
	{
		delete m_Camera;
		UnloadData();
		m_Renderer->Shutdown();
		SDL_Quit();
	}
	
	void Game::UpdateGame()
	{
		// DeltaTime calculations
		float deltaTime = (SDL_GetTicks() - m_TicksCount)/1000.0f;
		deltaTime = std::min(deltaTime, 0.05f);
		m_TicksCount = SDL_GetTicks();

		// Update all actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActors = false;

		// Move all pending actors to m_Actors
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);
		m_PendingActors.clear();

		// Delete Dead actors [the destructor of Actor will remove itself from m_Actors]
		std::vector<Actor*> actorsToBeDeleted;

		for (Actor* actor : m_Actors)
			if (actor->GetState() == Actor::EDead)
				actorsToBeDeleted.emplace_back(actor);

		for (Actor* actor : actorsToBeDeleted)
			delete actor;
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

		for (Actor* actor : m_Actors)
			actor->ProcessInput(keyState);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw(m_Camera);
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
			m_Actors.pop_back();
		}
		
		iter = std::find(m_PendingActors.begin(), m_PendingActors.end(), actor);
		if (iter != m_PendingActors.end())
		{
			std::iter_swap(iter, m_PendingActors.end() - 1);
			m_PendingActors.pop_back();
		}
	}

	void Game::LoadData()
	{
		TestActor* ta = new TestActor(this);

		m_Camera->SetPosition(glm::vec3(0, 0, 10));
	}
	
	void Game::UnloadData()
	{
	}
}