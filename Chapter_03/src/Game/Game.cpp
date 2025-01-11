#include "Game.h"
#include <iostream>

#include "Actor.h"
#include "SpriteComponent.h"
#include "Asteroid.h"
#include "Ship.h"

namespace ch3
{
	Game::Game()
	{
		m_IsRunnning = true;
		m_TicksCount = 0;
		m_UpdatingActors = false;
	}
	
	bool Game::init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_Window = SDL_CreateWindow("Chapter 03 - Vectors and Basic Physics", 50, 50, 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		if (!m_Window)
		{
			SDL_Log("Failed to Create a Window: %s", SDL_GetError());
			return false;
		}

		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!m_Renderer)
		{
			SDL_Log("Failed to Initialise the Renderer: %s", SDL_GetError());
			return false;
		}

		IMG_Init(IMG_INIT_PNG);

		loadData();
		return true;
	}
	
	void Game::runLoop()
	{
		while (m_IsRunnning)
		{
			processInput();
			updateGame();
			generateOutput();
		}
	}
	
	void Game::shutdown()
	{
		unloadData();
		IMG_Quit();
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}

	void Game::addActor(Actor* actor)
	{
		if (m_UpdatingActors)
			m_PendingActors.emplace_back(actor);
		else
			m_Actors.emplace_back(actor);
	}

	void Game::removeActor(Actor* actor)
	{
		auto tryRemoveActor = [](std::vector<Actor*>& vec, Actor* actor)
			{
				auto it = std::find(vec.begin(), vec.end(), actor);
				if (it != vec.end())
				{
					vec.erase(it);
					return true;
				}
				return false;
			};

		if (!tryRemoveActor(m_Actors, actor))
			tryRemoveActor(m_PendingActors, actor);
	}

	void Game::addSprite(SpriteComponent* sprite)
	{
		int drawOrder = sprite->getDrawOrder();
		auto it = m_Sprites.begin();

		for ( ; it != m_Sprites.end(); it++)
			if (drawOrder < (*it)->getDrawOrder())
				break;

		m_Sprites.insert(it, sprite);
	}

	void Game::removeSprite(SpriteComponent* sprite)
	{
		auto it = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);

		m_Sprites.erase(it);
	}

	void Game::loadData()
	{
		Ship* ship = new Ship(this);
		const int numAsteroids = 25;
		for (int i = 0; i < 20; i++)
			Asteroid* as = new Asteroid(this);
	}

	void Game::unloadData()
	{
		// Delete all actors, the destructor of actor automatically calls removeactor()
		while (!m_Actors.empty())
			delete m_Actors.back();

		// Destroy all textures
		for (auto texture : m_Textures)
			SDL_DestroyTexture(texture.second);
		m_Textures.clear();
	}

	SDL_Texture* Game::getTexture(const char* filename)
	{
		SDL_Texture* texture;

		auto it = m_Textures.find(filename);
		if (it != m_Textures.end())
		{
			texture = it->second;
		}
		else
		{
			SDL_Surface* surface = IMG_Load(filename);
			if (!surface)
			{
				SDL_Log("Failed to load the texture at '%s'", filename);
				return nullptr;
			}

			texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
			if (!texture)
			{
				SDL_Log("Failed to create the texture from '%s'", filename);
				return nullptr;
			}
			m_Textures.emplace(filename, texture);
		}

		return texture;
	}
	
	void Game::processInput()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_IsRunnning = false;
				break;
			default:
				break;
			}
		}

		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		if (keyboardState[SDL_SCANCODE_ESCAPE])
			m_IsRunnning = false;

		m_UpdatingActors = true;
		for (auto actor : m_Actors)
			actor->processInput(keyboardState);
		m_UpdatingActors = false;
	}
	
	void Game::updateGame()
	{
		// DeltaTime calculation
		float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
		deltaTime = deltaTime > 0.5f ? 0.5f : deltaTime;
		m_TicksCount = SDL_GetTicks();

		// Update Actors
		m_UpdatingActors = true;
		for (auto actor : m_Actors)
			actor->update(deltaTime);
		m_UpdatingActors = false;

		// Move pending actors to m_Actors
		for (auto pendingActor : m_PendingActors)
			m_Actors.emplace_back(pendingActor);
		m_PendingActors.clear();

		// Delete dead actors
		for (auto it = m_Actors.begin(); it != m_Actors.end(); )
		{
			if ((*it)->getState() == Actor::EDead)
			{
				delete* it;
				it = m_Actors.erase(it);
			}
			else
				it++;
		}
	}

	void Game::generateOutput()
	{
		SDL_SetRenderDrawColor(m_Renderer, 100, 100, 100, 255);
		SDL_RenderClear(m_Renderer);

		for (auto sprite : m_Sprites)
			sprite->draw(m_Renderer);

		// Swap Buffers
		SDL_RenderPresent(m_Renderer);
	}
}