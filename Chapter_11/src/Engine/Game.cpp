#include "Game.h"

#include <algorithm>
#include "Renderer.h"
#include "InputSystem.h"
#include "Actor.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_UpdatingActors = false;
		m_TicksCount = SDL_GetTicks();
		m_Renderer = new Renderer(this);
		m_InputSystem = new InputSystem();
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

		if (!m_Renderer->Init(1280, 720))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise Renderer");
			delete m_Renderer;
			m_Renderer = nullptr;
			return false;
		}

		m_InputSystem->Init();

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
		m_Renderer->Shutdown();
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
				m_IsRunning = false;
				break;
			case SDL_MOUSEWHEEL:
				m_InputSystem->ProcessEvent(event);
				break;
			default:
				break;
			}
		}

		m_InputSystem->Update();

		InputState inputState = m_InputSystem->GetInputState();

		for (Actor* actor : m_Actors)
			actor->Input(inputState);

		if (inputState.Keyboard.GetKeyUp(SDL_SCANCODE_ESCAPE))
			m_IsRunning = false;
	}
	
	void Game::UpdateGame()
	{
		// Frame cap at 60fps
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));
		// Get deltaTime
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;
		m_TicksCount = SDL_GetTicks();

		// Update Actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->UpdateActor(deltaTime);
		m_UpdatingActors = false;

		// Move pending actors to m_Actors, and clear pending actors
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);
		m_PendingActors.clear();

		// gather any dead actors to temp list
		std::vector<Actor*> deadActors;
		for (Actor* actor : m_Actors)
			if (actor->GetState() == Actor::E_Dead)
				deadActors.emplace_back(actor);

		// delete all dead actors
		for (Actor* actor : deadActors)
			delete actor;
		deadActors.clear();
	}
	
	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}

}