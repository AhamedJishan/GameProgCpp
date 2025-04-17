#include "Game.h"

#include <GLEW/GL/glew.h>
#include <algorithm>

#include "Actor.h"
#include "VertexArray.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Texture.h"

#include "MoveComponent.h"
#include "Game/Ship.h"
#include "Game/Asteroid.h"
#include "Game/Laser.h"

namespace jLab
{
	Game::Game()
	{
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Window = nullptr;
		m_UpdatingActors = false;
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("Failed to initialise SDL: %s", SDL_GetError());
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_Window = SDL_CreateWindow("Chapter_05: Opengl", 100, 100, 1280, 720, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_Log("Failed to create Window: %s", SDL_GetError());
			return false;
		}

		m_Context = SDL_GL_CreateContext(m_Window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			SDL_Log("Failed to Initialise GLEW");
			return false;
		}
		glGetError();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		InitSpriteVerts();
		if (!InitShaders())
		{
			SDL_Log("Failed to Initialise Shaders");
			return false;
		}

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
	
	void Game::ShutDown()
	{
		UnloadData();

		m_SpriteShader->Unload();
		delete m_SpriteShader;
		delete m_SpriteVerts;

		SDL_GL_DeleteContext(m_Context);
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
		int drawOrder = sprite->GetDrawOrder();

		auto iter = m_Sprites.begin();
		for (; iter != m_Sprites.end(); iter++)
			if (drawOrder < (*iter)->GetDrawOrder())
				break;
		m_Sprites.insert(iter, sprite);
	}

	void Game::RemoveSprite(SpriteComponent* sprite)
	{
		auto iter = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
		if (iter != m_Sprites.end())
			m_Sprites.erase(iter);
	}

	Texture* Game::GetTexture(const std::string& filename)
	{
		Texture* tex = nullptr;

		auto iter = m_Textures.find(filename);
		if (iter != m_Textures.end())
		{
			tex = iter->second;
		}
		else
		{
			tex = new Texture();
			if (tex->Load(filename))
				m_Textures.emplace(filename, tex);
			else
			{
				delete tex;
				return nullptr;
			}
		}

		return tex;
	}

	void Game::AddAsteroid(Asteroid* asteroid)
	{
		m_Asteroids.emplace_back(asteroid);
	}

	void Game::RemoveAsteroid(Asteroid* asteroid)
	{
		auto iter = std::find(m_Asteroids.begin(), m_Asteroids.end(), asteroid);
		if (iter != m_Asteroids.end())
			m_Asteroids.erase(iter);
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

		const Uint8* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;

		for (Actor* actor : m_Actors)
			actor->ProcessInput(keyState);
	}
	
	void Game::UpdateGame()
	{
		// Delta time calculations
		float deltaTime = (SDL_GetTicks() - m_TicksCount)/1000.0f;
		deltaTime = deltaTime > 0.05f ? 0.05f : deltaTime;
		m_TicksCount = SDL_GetTicks();

		// Updating all the actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
		{
			actor->Update(deltaTime);
		}
		m_UpdatingActors = false;

		// Moving pending actors to main actors list
		for (Actor* actor : m_PendingActors)
		{
			actor->CalculateWorldTransform();
			m_Actors.emplace_back(actor);
		}
		m_PendingActors.clear();

		// Moving dead actors to temp list
		std::vector<Actor*> deadActors;
		for (auto actor : m_Actors)
		{
			if (actor->GetState() == Actor::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}

		// Delete dead actors (which removes them from mActors)
		for (auto actor : deadActors)
		{
 			delete actor;
		}
	}
	
	void Game::GenerateOutput()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_SpriteVerts->SetActive();
		for (SpriteComponent* sprite : m_Sprites)
			sprite->Draw(m_SpriteShader);

		SDL_GL_SwapWindow(m_Window);
	}
	
	void Game::InitSpriteVerts()
	{
		float vertices[] = {
			-0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
			 0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
			 0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
			-0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_SpriteVerts = new VertexArray(vertices, 4, indices, 6);
	}

	bool Game::InitShaders()
	{
		m_SpriteShader = new Shader();

		if (!m_SpriteShader->Load("src/Engine/Shaders/Sprite.vert", "src/Engine/Shaders/Sprite.frag"))
			return false;

		m_SpriteShader->SetActive();

		Matrix4 viewProj = Matrix4::CreateSimpleViewProj(1280, 720);
		m_SpriteShader->SetMatrixUniform("u_ViewProj", viewProj);
	}

	void Game::LoadData()
	{
		m_Ship = new Ship(this);

		const int numAsteroids = 20;
		for (int i = 0; i < numAsteroids; i++)
			new Asteroid(this);
	}

	void Game::UnloadData()
	{
		while (m_Actors.empty())
			delete m_Actors.back();
		m_Actors.clear();

		for (auto i : m_Textures)
			delete i.second;
		m_Textures.clear();
	}

}