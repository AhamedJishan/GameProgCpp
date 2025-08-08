#include "Game.h"

#include <algorithm>
#include <SDL/SDL_ttf.h>
#include "Renderer.h"
#include "InputSystem.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "PhysWorld.h"
#include "Font.h"
#include "UIScreen.h"

#include "Game/WallActor.h"
#include "Game/GroundActor.h"
#include "Game/FPSActor.h"
#include "Game/CrosshairActor.h"
#include "Game/RadarActor.h"
#include "Game/TargetActor.h"
#include "Game/PauseMenu.h"

namespace jLab
{
	Game::Game()
	{
		m_TicksCount = 0;
		m_GameState = E_Gameplay;
		m_UpdatingActors = false;
		m_InputSystem = new InputSystem();
		m_AudioSystem = new AudioSystem(this);
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

		if (TTF_Init() != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialize SDL TTF: %s", TTF_GetError());
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
		m_AudioSystem->Init();
		m_InputSystem->SetRelativeMouseMode(true);

		LoadData();

		return true;
	}

	void Game::Shutdown()
	{
		UnloadData();
		m_InputSystem->Shutdown();
		m_AudioSystem->Shutdown();
		m_Renderer->Shutdown();
		TTF_Quit();
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

	Font* Game::GetFont(const std::string& filename)
	{
		auto iter = m_Fonts.find(filename);
		if (iter != m_Fonts.end())
		{
			return iter->second;
		}
		else
		{
			Font* font = new Font();
			if (font->Load(filename))
				m_Fonts.emplace(filename, font);
			else
			{
				font->Unload();
				delete font;
				font = nullptr;
			}

			return font;
		}
	}

	void Game::PushUI(UIScreen* screen)
	{
		m_UIStack.emplace_back(screen);
	}

	void Game::Run()
	{
		while (m_GameState != E_Quit)
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
				m_GameState = E_Quit;
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

		//if (inputState.Keyboard.GetKeyUp(SDL_SCANCODE_ESCAPE))
		//	m_GameState = E_Quit;

		if(m_GameState == E_Gameplay)
		{
			m_UpdatingActors = true;
			for (Actor* actor : m_Actors)
				actor->ProcessInput(inputState);
			m_UpdatingActors = false;
		}
		else if (!m_UIStack.empty())
		{
			m_UIStack.back()->ProcessInput(inputState);
		}

		// GAME SPECIFIC
		if (m_GameState == E_Gameplay)
			if (inputState.Keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
				PauseMenu* pm = new PauseMenu(this);
	}

	void Game::UpdateGame()
	{
		// Cap FPS at 60
		//while (!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;

		if (deltaTime > 0.05f)
			deltaTime = 0.05f;

		m_TicksCount = SDL_GetTicks();

		if (m_GameState == E_Gameplay)
		{
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

		// Update AudioSystem
		m_AudioSystem->Update();

		// Update UI
		for (UIScreen* ui : m_UIStack)
			if (ui->GetState() == UIScreen::E_Active)
				ui->Update(deltaTime);
		// Delete closing UI
		auto iter = m_UIStack.begin();
		while (iter != m_UIStack.end())
		{
			if ((*iter)->GetState() == UIScreen::E_Closing)
			{
				delete (*iter);
				iter = m_UIStack.erase(iter);
			}
			else
				iter++;
		}
	}

	void Game::GenerateOutput()
	{
		m_Renderer->Draw();
	}

	void Game::LoadData()
	{
		FPSActor* fpsActor = new FPSActor(this);
		fpsActor->SetPosition(glm::vec3(0, 1, 0));

		CrosshairActor* ca = new CrosshairActor(this);
		RadarActor* ra = new RadarActor(this);
		ra->SetScale(glm::vec3(0.75f, 0.75f, 1.0f));
		ra->SetPosition(glm::vec3(-540.0f, 260.0f, 0.0f));

		TargetActor* ta1 = new TargetActor(this);
		TargetActor* ta2 = new TargetActor(this);
		TargetActor* ta3 = new TargetActor(this);
		TargetActor* ta4 = new TargetActor(this);
		ta1->SetPosition(glm::vec3(0, 1.2f, -9.5f));
		ta2->SetPosition(glm::vec3(0, 3.0f, -9.5f));
		ta3->SetPosition(glm::vec3(-1.5f, 2.1f, -9.5f));
		ta4->SetPosition(glm::vec3(1.5f, 2.1f, -9.5f));

		GroundActor* ga = new GroundActor(this);

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