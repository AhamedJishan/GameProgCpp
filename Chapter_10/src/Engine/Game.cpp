#include "Game.h"

#include <algorithm>
#include "Renderer.h"
#include "InputSystem.h"
#include "Actor.h"
#include "PhysWorld.h"

#include "Game/WallActor.h"
#include "Game/TestActor.h"
#include "Game/FPSActor.h"

namespace jLab
{
	Game::Game()
	{
		m_TicksCount = 0;
		m_IsRunning = true;
		m_UpdatingActors = false;
		m_InputSystem = new InputSystem();
		m_Renderer = new Renderer(this);
		m_PhysWorld = new PhysWorld(this);
	}

	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialize SDL");
			return false;
		}

		if (!m_Renderer->Init(1280, 720))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize Renderer");
			delete m_Renderer;
			m_Renderer = nullptr;
			return false;
		}

		m_InputSystem->Init();
		m_InputSystem->SetRelativeMouseMode(true);

		// TODO: Init AudioSystem

		LoadData();

		return true;
	}

	void Game::Shutdown()
	{
		UnloadData();
		m_InputSystem->Shutdown();
		m_Renderer->Shutdown();
		SDL_Quit();
	}

	void Game::AddActor(Actor* actor)
	{
		if (m_UpdatingActors)
			m_PendingActors.push_back(actor);
		else
			m_Actors.push_back(actor);
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

	void Game::Run()
	{
		while (m_IsRunning)
		{
			ProcessInput();
			UpdateGame();
			GenerateOutput();
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
				m_IsRunning = false;
				break;
			case SDL_MOUSEWHEEL:
				m_InputSystem->ProcessInput(event);
				break;
			default:
				break;
			}
		}

		m_InputSystem->Update();

		InputState inputState = m_InputSystem->GetState();

		if (inputState.Keyboard.GetKeyUp(SDL_SCANCODE_ESCAPE))
		{
			m_IsRunning = false;
		}

		for (Actor* actor : m_Actors)
			actor->ProcessInput(inputState);
	}
	
	void Game::UpdateGame()
	{
		// Cap FPS at 60
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;

		if (deltaTime > 0.05f)
			deltaTime = 0.05f;

		m_TicksCount = SDL_GetTicks();

		// Update all actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActors = false;

		// Move pending to actors to m_Actors
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);
		m_PendingActors.clear();

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
		fpsActor->SetPosition(glm::vec3(0, 1, 0));

		TestActor* ta = new TestActor(this);

		WallActor* wa1 = new WallActor(this);
		WallActor* wa2 = new WallActor(this);
		WallActor* wa3 = new WallActor(this);
		WallActor* wa4 = new WallActor(this);

		wa1->SetPosition(glm::vec3(10, 0, 0));
		wa2->SetPosition(glm::vec3(-10, 0, 0));
		wa3->SetPosition(glm::vec3(0, 0, 10));
		wa4->SetPosition(glm::vec3(0, 0, -10));

		wa3->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
		wa4->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));

		m_WallPlanes.emplace_back(wa1);
		m_WallPlanes.emplace_back(wa2);
		m_WallPlanes.emplace_back(wa3);
		m_WallPlanes.emplace_back(wa4);
	}
	
	void Game::UnloadData()
	{
	}
}