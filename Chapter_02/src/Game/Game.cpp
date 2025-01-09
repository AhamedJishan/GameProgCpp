#include "Game.h"

#include <SDL_image.h>
#include <algorithm>
#include "Ship.h"
#include "BGSpriteComponent.h"

namespace ch2
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
	}

	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		m_Window = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1280, 720, 0);
		if (!m_Window)
		{
			SDL_Log("Failed to create window: %s", SDL_GetError());
			return false;
		}

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_Renderer)
		{
			SDL_Log("Failed to create renderer: %s", SDL_GetError());
			return false;
		}

		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
			return false;
		}

		// LOAD DATA
		LoadData();

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
		UnLoadData();
		IMG_Quit();
		SDL_DestroyRenderer(m_Renderer);
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
		auto TryRemoveActor = [](std::vector<Actor*>& vec, Actor* actor)
			{
				auto it = std::find(vec.begin(), vec.end(), actor);
				if (it != vec.end())
				{
					std::iter_swap(it, vec.end() - 1);
					vec.pop_back();
					return true;
				}
				
				return false;
			};

		if (!TryRemoveActor(m_Actors, actor))
			TryRemoveActor(m_PendingActors, actor);
	}

	void Game::AddSprite(SpriteComponent* sprite)
	{
		int drawOrder = sprite->GetDrawOrder();

		auto it = m_Sprites.begin();

		for ( ; it != m_Sprites.end(); it++)
			if (drawOrder < (*it)->GetDrawOrder())
				break;

		m_Sprites.insert(it, sprite);
	}

	void Game::RemoveSprite(SpriteComponent* sprite)
	{
		auto it = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
		if (it != m_Sprites.end())
		{
			m_Sprites.erase(it);
		}
	}

	void Game::LoadData()
	{
		m_Ship = new Ship(this);
		m_Ship->SetPosition(Vector2(100.0f, 720.0f / 2.0f));
		m_Ship->SetScale(1.5f);

		Actor* temp = new Actor(this);
		std::vector<SDL_Texture*> textures = {
			GetTexture("Assets/Farback01.png"),
			GetTexture("Assets/Farback02.png")
		};
		BGSpriteComponent* bgs = new BGSpriteComponent(temp);
		bgs->SetScreenSize(Vector2(1280, 720));
		bgs->SetBGTexture(textures);
		bgs->SetScrollSpeed(-100.0f);

		temp = new Actor(this);
		textures = {
			GetTexture("Assets/Stars.png"),
			GetTexture("Assets/Stars.png")
		};
		bgs = new BGSpriteComponent(temp, 50);
		bgs->SetScreenSize(Vector2(1280, 720));
		bgs->SetBGTexture(textures);
		bgs->SetScrollSpeed(-200.0f);
	}

	void Game::UnLoadData()
	{
		while (!m_Actors.empty())
			delete m_Actors.back();

		for (auto i : m_Textures)
			SDL_DestroyTexture(i.second);
		m_Textures.clear();
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
				SDL_Log("Failed to load the texture: %s", filename);
				return nullptr;
			}

			texture = SDL_CreateTextureFromSurface(m_Renderer, surf);
			SDL_FreeSurface(surf);
			if (!texture)
			{
				SDL_Log("Failed to convert sdl surface to texture: ", filename);
				return nullptr;
			}

			m_Textures.emplace(filename, texture);
		}

		return texture;
	}

	void Game::ProcessInput()
	{
		SDL_Event event;

		// While there are still events in the queue
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_IsRunning = false;
				break;
			}
		}

		// Keyboard Input
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		if (keyboardState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;

		m_Ship->ProcessInput(keyboardState);
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
		SDL_SetRenderDrawColor(m_Renderer, 20, 20, 20, 255);
		SDL_RenderClear(m_Renderer);

		for (SpriteComponent* sprite : m_Sprites)
			sprite->Draw(m_Renderer);

		// swap buffers
		SDL_RenderPresent(m_Renderer);
	}
}