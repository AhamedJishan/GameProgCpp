#include "Game.h"

#include <SDL_image.h>

namespace ch2
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
	}
	Game::~Game()
	{
	}

	bool Game::Init()
	{
		int sdlResult = SDL_Init(SDL_INIT_VIDEO);
		if (sdlResult != 0)
		{
			SDL_Log("Failed to Initialise SDL!");
			return false;
		}

		IMG_Init(IMG_INIT_PNG);

		m_Window = SDL_CreateWindow("Chapter_02", 100, 50, 1280, 720, 0);
		if (!m_Window)
		{
			SDL_Log("Failed to create SDL Window!");
			return false;
		}

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_Renderer)
		{
			SDL_Log("Failed to Create SDL Renderer!");
			return false;
		}

		return true;
	}

	void Game::RunLoop()
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
		while (!m_Actors.empty())
			delete m_Actors.back();

		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	void Game::AddActor(Actor* actor)
	{
		if (m_UpdatingActor)
			m_PendingActors.emplace_back(actor);
		else
			m_Actors.emplace_back(actor);
	}

	void Game::RemoveActor(Actor* actor)
	{
		auto TryRemoveActor = [](std::vector<Actor*> vec, Actor* actor)
			{
				auto it = std::find(vec.begin(), vec.end(), actor);
				if (it != vec.end())
				{
					vec.erase(it);
					return true;
				}
				
				return false;
			};

		if (!TryRemoveActor(m_Actors, actor))
			TryRemoveActor(m_PendingActors, actor);
	}

	SDL_Texture* Game::GetTexture(const char* filename)
	{
		SDL_Texture* texture;

		auto it = m_Textures.find(filename);
		if (it != m_Textures.end())
		{
			texture = it->second;
		}
		else
		{
			SDL_Surface* surf = IMG_Load(filename);
			if (!surf)
			{
				SDL_Log("Failed to load the texture: ", filename);
				return nullptr;
			}

			texture = SDL_CreateTextureFromSurface(m_Renderer, surf);
			SDL_FreeSurface(surf);
			if (!texture)
			{
				SDL_Log("Failed to convert sdl surface to texture: ", filename);
				return nullptr;
			}
		}

		return texture;
	}

	void Game::ProcessInput()
	{
	}

	void Game::UpdateGame()
	{
		// DeltaTime calculation
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = deltaTime > 0.5f ? 0.5f : deltaTime;
		m_TicksCount = SDL_GetTicks();

		// Update actors
		m_UpdatingActor = true;
		for (auto actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActor = false;

		// Move pending actors to main actors list
		for (auto actor : m_PendingActors)
			m_Actors.emplace_back(actor);
		m_PendingActors.clear();

		// Delete dead actors
		std::vector<Actor*> deadActors;
		for (auto actor : m_Actors)
		{
			if (actor->GetState() == Actor::EDead)
				deadActors.emplace_back(actor);
		}
		for (auto actor : deadActors)
			delete actor;
	}

	void Game::GenerateOutput()
	{
	}
}