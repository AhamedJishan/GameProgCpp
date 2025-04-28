#include "Game.h"

#include <SDL/SDL.h>
#include <algorithm>
#include "Renderer.h"
#include "Actor.h"
#include "Camera.h"

#include "Game/BagActor.h"
#include "Game/ContainerActor.h"
#include "Game/SceneActor.h"
#include "Game/radarActor.h"
#include "Game/HealthBarActor.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Renderer = new Renderer(this);
		m_Camera = new Camera(this, 1280, 720, 0.1f, 1000.0f, 80.0f);
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialise SDL : % s", SDL_GetError());
			return false;
		}

		m_Renderer->Init(1280, 720);

		// TODO: Initialise AudioSystem

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
	
	void Game::Shutdown()
	{
		UnloadData();
		delete m_Camera;
		delete m_Renderer;
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
		
		for (Actor* actor : m_Actors)
			actor->ProcessInput(keyState);
	}
	
	void Game::UpdateGame()
	{
		// DeltaTime Calculations
		float deltaTime = (SDL_GetTicks() - m_TicksCount)/1000.0f;
		deltaTime = std::min(deltaTime, 0.05f);
		m_TicksCount = SDL_GetTicks();

		// UPDATE all actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActors = false;

		// MOVE pending actors to m_Actors
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);
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

		// Lock FPS at 60
		while (SDL_GetTicks() < m_TicksCount + 16);
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
			m_Actors.pop_back();
			return;
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
		BagActor* ba = new BagActor(this);
		ba->SetPosition(glm::vec3(1, 0, -5));
		ba->SetScale(glm::vec3(0.5f));

		ContainerActor* ca = new ContainerActor(this);
		ca->SetPosition(glm::vec3(-1, 0, 0));

		SceneActor* sa = new SceneActor(this);
		sa->SetPosition(glm::vec3(0, -1, 0));

		RadarActor* ra = new RadarActor(this);
		ra->SetScale(glm::vec3(0.75f));
		ra->SetPosition(glm::vec3(530.0f, -250.0f, 0.0f));

		HealthBarActor* ha = new HealthBarActor(this);
		ha->SetPosition(glm::vec3(-485.0f, -320.0f, 0.0f));

		m_Camera->SetPosition(glm::vec3(0, 0, 5));
	}

	void Game::UnloadData()
	{
	}
}