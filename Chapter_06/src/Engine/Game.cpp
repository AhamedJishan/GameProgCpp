#include "Game.h"

#include <GLEW/GL/glew.h>
#include <algorithm>
#include "Actor.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Component/MeshComponent.h"
#include "Model.h"

#include "Game/TestActor.h"

namespace jLab
{
	Game::Game()
	{
		m_Window = nullptr;
		m_Context = NULL;
		m_IsRunning = true;
		m_TicksCount = 0;
		m_Shader = nullptr;
		m_Camera = nullptr;
	}
	
	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			SDL_Log("ERROR: Failed To Initialise SDL: %s", SDL_GetError());
			return false;
		}

		m_Window = SDL_CreateWindow("Chapter_06: 3D OpenGl", 100, 30, 1280, 720, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_Log("ERROR: Failed to Create Window: %s", SDL_GetError());
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

		m_Context = SDL_GL_CreateContext(m_Window);

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			SDL_Log("ERROR: Failed to Initialise GLEW");
			return false;
		}
		glGetError();

		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
		glEnable(GL_DEPTH_TEST);

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
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
		SDL_Quit();
	}
	
	void Game::UpdateGame()
	{
		// DeltaTime calculations
		float deltaTime = (SDL_GetTicks() - m_TicksCount)/1000.0f;
		deltaTime = std::min(deltaTime, 0.05f);
		m_TicksCount = SDL_GetTicks();

		// Update all actors
		m_UpdatingActors = true;
		for (Actor* actor : m_Actors)
			actor->Update(deltaTime);
		m_UpdatingActors = false;

		// Move all pending actors to m_Actors
		for (Actor* actor : m_PendingActors)
			m_Actors.emplace_back(actor);
		m_PendingActors.clear();

		// Delete Dead actors [the destructor of Actor will remove itself from m_Actors]
		std::vector<Actor*> actorsToBeDeleted;

		for (Actor* actor : m_Actors)
			if (actor->GetState() == Actor::EDead)
				actorsToBeDeleted.emplace_back(actor);

		for (Actor* actor : actorsToBeDeleted)
			delete actor;
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

		const uint8_t* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			m_IsRunning = false;
	}
	
	void Game::GenerateOutput()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TODO: Render scene
		m_Shader->SetActive();
		m_Shader->SetMat4("uViewProjection", m_Camera->GetViewProjMatrix());
		for (MeshComponent* mesh : m_Meshes)
			mesh->Draw(m_Shader);

		SDL_GL_SwapWindow(m_Window);
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

	void Game::AddMeshComponent(MeshComponent* mesh)
	{
		m_Meshes.emplace_back(mesh);
	}

	void Game::RemoveMeshComponent(MeshComponent* mesh)
	{
		auto iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
		if (iter != m_Meshes.end())
			m_Meshes.erase(iter);
	}


	Texture* Game::GetTexture(const std::string& filename, Texture::TextureType type)
	{
		auto iter = m_Textures.find(filename);
		if (iter != m_Textures.end())
		{
			return iter->second;
		}

		Texture* texture = new Texture();
		texture->SetTextureType(type);
		if (texture->Load(filename))
		{
			m_Textures.emplace(filename, texture);
		}
		else
		{
			delete texture;
			return nullptr;
		}

		return texture;
	}

	void Game::LoadData()
	{
		m_Camera = new Camera(this, 1280, 720, 0.1f, 100.0f, 45.0f);
		m_Camera->SetPosition(glm::vec3(0, 0, 10));
		m_Shader = new Shader("Assets/Shaders/BasicMesh.vert", "Assets/Shaders/BasicMesh.frag");

		TestActor* ta = new TestActor(this);
	}
	
	void Game::UnloadData()
	{
		delete m_Camera;
		delete m_Shader;
	}
}