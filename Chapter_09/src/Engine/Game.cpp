#include "Game.h"

#include <SDL/SDL.h>
#include <algorithm>
#include "Renderer.h"
#include "InputSystem.h"
#include "Actor.h"

#include "Game/SceneActor.h"
#include "Game/FPSActor.h"
#include "Game/FollowActor.h"
#include "Engine/Components/FollowCamera.h"
#include "Game/OrbitActor.h"
#include "Game/SplineActor.h"
#include "Game/ProjectionArrowActor.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_UpdatingActors = false;
		m_InputSystem = new InputSystem();
		m_Renderer = new Renderer(this);
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_InputSystem->Init();
		m_InputSystem->SetRelativeMouseMode(true);
		m_Renderer->Init(SCREEN_WIDTH, SCREEN_HEIGHT);

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
		if (m_Renderer)
		{
			m_Renderer->Shutdown();
			delete m_Renderer;
		}
		SDL_Quit();
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
	
	void Game::ProcessInput()
	{
		m_InputSystem->PreUpdate();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				m_IsRunning = false;
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				m_InputSystem->ProcessInput(event);
				break;
			}
			default:
				break;
			}
		}

		m_InputSystem->Update();

		InputState inputState = m_InputSystem->GetState();

		if (inputState.Keyboard.GetKeyUp(SDL_SCANCODE_ESCAPE)) m_IsRunning = false;

		for (Actor* actor : m_Actors)
			actor->ProcessInput(inputState);
	}
	
	void Game::UpdateGame()
	{
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = std::min(0.05f, deltaTime);
		m_TicksCount = SDL_GetTicks();
		// FPS lock at around 60fps
		while (SDL_GetTicks() < (m_TicksCount + 16));

		// Update all actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->UpdateActor(deltaTime);
		m_UpdatingActors = true;

		// Move pending to actors to m_Actors
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);

		// Move dead actors into a temp list, then delete them
		std::vector<Actor*> deadActors;
		for (Actor* actor : m_Actors)
			if (actor->GetState() == Actor::E_Dead)
				deadActors.emplace_back(actor);

		// Destructor of actors removes itself from m_Actors
		for (Actor* actor : deadActors)
			delete actor;
		deadActors.clear();
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}
	
	void Game::LoadData()
	{
		FPSActor* fpsActor = new FPSActor(this);
		   
		// FollowActor* followActor = new FollowActor(this);
		// followActor->SetPosition(glm::vec3(0, -1, 0));
		// followActor->GetFollowCamera()->SnapToIdeal();
		   
		// OrbitActor* orbitActor = new OrbitActor(this);
		// orbitActor->SetPosition(glm::vec3(0, -1, 0));

		//SplineActor* splineActor = new SplineActor(this);

		ProjectionArrowActor* projectionArrowActor = new ProjectionArrowActor(this);
		projectionArrowActor->SetPosition(glm::vec3(0, -5, 0));

		SceneActor* sa = new SceneActor(this);
		sa->SetPosition(glm::vec3(0, -1, 0));
		sa->SetScale(glm::vec3(2));
	}

	void Game::UnloadData()
	{
		while (!m_Actors.empty())
			delete m_Actors.back();

		while (!m_PendingActors.empty())
			delete m_PendingActors.back();
	}
}