#include "Game.h"

#include <SDL/SDL_image.h>
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"

#include "MoveComponent.h"

namespace jLab
{
	Game::Game()
		:m_Renderer(nullptr),
		m_Window(nullptr),
		m_IsRunning(false),
		m_UpdatingActors(false)
	{
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Unable to initialise SDL '%s'", SDL_GetError());
			return false;
		}
		m_Window = SDL_CreateWindow("Chapter_04", 100, 100, 1280, 720, 0);
		if (!m_Window)
		{
			SDL_Log("Unable to create Window '%s'", SDL_GetError());
			return false;
		}
		m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		if (!m_Renderer)
		{
			SDL_Log("Unable to create Renderer '%s'", SDL_GetError());
			return false;
		}
		if (IMG_Init(IMG_INIT_PNG) == 0)
		{
			SDL_Log("Unable to initialise SDL_Image '%s'", SDL_GetError());
			return false;
		}

		LoadData();
		m_IsRunning = true;
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
		UnloadData();
		IMG_Quit();
		SDL_DestroyRenderer(m_Renderer);
		SDL_DestroyWindow(m_Window);
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
		auto iter = std::find(m_PendingActors.begin(), m_PendingActors.end(), actor);
		if (iter != m_PendingActors.end())
		{
			std::iter_swap(iter, m_PendingActors.end() - 1);
			m_PendingActors.pop_back();
		}

		iter = std::find(m_Actors.begin(), m_Actors.end(), actor);
		if (iter != m_Actors.end())
		{
			std::iter_swap(iter, m_Actors.end() - 1);
			m_Actors.pop_back();
		}
	}

	void Game::AddSprite(SpriteComponent* sprite)
	{
		int updateOrder = sprite->GetUpdateOrder();
		auto iter = m_Sprites.begin();
		for (; iter != m_Sprites.end(); iter++)
		{
			if (updateOrder < (*iter)->GetUpdateOrder())
				break;
		}
		m_Sprites.insert(iter, sprite);
	}

	void Game::RemoveSprite(SpriteComponent* sprite)
	{
		auto iter = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
		if (iter != m_Sprites.end())
			m_Sprites.erase(iter);
		else
			SDL_Log("Tried to remove a sprite that is not in list");
	}
	
	SDL_Texture* Game::GetTexture(const std::string& filename)
	{
		SDL_Texture* tex = nullptr;

		auto iter = m_Textures.find(filename);
		if (iter != m_Textures.end())
			tex = iter->second;
		else
		{
			SDL_Surface* surf = IMG_Load(filename.c_str());
			if (!surf)
			{
				SDL_Log("Failed to load texture file from '%s'", filename);
				return tex;
			}
			tex = SDL_CreateTextureFromSurface(m_Renderer, surf);
			SDL_FreeSurface(surf);
			if (!tex)
			{
				SDL_Log("Failed to create texture file for '%s'", filename);
				return tex;
			}

			m_Textures.emplace(filename.c_str(), tex);
		}

		return tex;
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
			}
		}

		const Uint8* keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;

		// Process Input for all the actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->ProcessInput(keyState);
		m_UpdatingActors = false;
	}
	
	void Game::UpdateGame()
	{
		// DeltaTime Calculations
		Uint32 currentTicks = SDL_GetTicks();
		float deltaTime = (currentTicks - m_TicksCount) / 1000.0f;
		if (deltaTime > 0.05f)
			deltaTime = 0.05f;
		m_TicksCount = currentTicks;

		// Update Actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActors = false;
		// Move pending actors to main actors list
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);
		m_PendingActors.clear();
		// Erase dead actors
		m_Actors.erase( std::remove_if(m_Actors.begin(), m_Actors.end(), [](Actor* actor) { return actor->GetState() == Actor::EDead;}),
			m_Actors.end() );
	}
	
	void Game::GenerateOutput()
	{
		SDL_SetRenderDrawColor(m_Renderer, 20, 20, 20, 255);
		SDL_RenderClear(m_Renderer);

		for (SpriteComponent* sprite : m_Sprites)
			sprite->Draw(m_Renderer);

		SDL_RenderPresent(m_Renderer);
	}
	
	void Game::LoadData()
	{
		Actor* actor = new Actor(this);
		actor->SetPosition(Vector2(500, 350));
		SpriteComponent* sc = new SpriteComponent(actor);
		sc->SetTexture(GetTexture("Assets/Airplane.png"));

		MoveComponent* mc = new MoveComponent(actor);
		mc->SetAngularSpeed(10);
		mc->SetForwardSpeed(1000);
	}
	
	void Game::UnloadData()
	{
	}
}