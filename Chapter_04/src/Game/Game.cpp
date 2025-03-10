#include "Game.h"

#include <SDL/SDL_image.h>
#include <algorithm>

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
	}
	
	void Game::UpdateGame()
	{
	}
	
	void Game::GenerateOutput()
	{
		SDL_SetRenderDrawColor(m_Renderer, 20, 20, 20, 255);
		SDL_RenderClear(m_Renderer);


		SDL_RenderPresent(m_Renderer);
	}
	
	void Game::LoadData()
	{
	}
	
	void Game::UnloadData()
	{
	}
}