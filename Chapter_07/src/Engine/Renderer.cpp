#include "Renderer.h"

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>
#include "Game.h"
#include "Shader.h"
#include "Camera.h"
#include "Component/MeshRenderer.h"
#include "Component/SpriteComponent.h"
#include "Model.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
		:m_Game(game),
		m_Context(nullptr),
		m_Window(nullptr),
		m_Width(0),
		m_Height(0)
	{
	}
	
	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		m_Width = screenWidth;
		m_Height = screenHeight;

		m_Window = SDL_CreateWindow("Chapter_07: Audio", 100, 30, m_Width, m_Height, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		m_Context = SDL_GL_CreateContext(m_Window);

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise GLEW");
			return false;
		}
		glGetError();

		glViewport(0, 0, m_Width, m_Height);

		LoadShaders();
		InitSpriteVerts();

		return true;
	}
	
	void Renderer::Shutdown()
	{
		glDeleteVertexArrays(1, &m_SpriteVAO);
		glDeleteBuffers(1, &m_SpriteVBO);
		glDeleteBuffers(1, &m_SpriteEBO);
		delete m_MeshShader;
		delete m_SpriteShader;

		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
	}
	
	void Renderer::Draw()
	{
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		m_MeshShader->SetActive();
		m_MeshShader->SetMat4("u_ViewProjection", m_Game->GetCamera()->GetViewProjMatrix());
		m_MeshShader->SetVec3("u_CameraPos", m_Game->GetCamera()->GetPosition());
		m_MeshShader->SetVec3("u_LightColor", glm::vec3(1.0f));
		m_MeshShader->SetVec3("u_LightDir", glm::vec3(1, -0.5f, -1));
		m_MeshShader->SetVec3("u_AmbientColor", glm::vec3(0.2f, 0.2f, 0.25f));

		for (MeshRenderer* mesh : m_Meshes)
			mesh->Draw(m_MeshShader);


		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_SpriteShader->SetActive();
		m_SpriteShader->SetMat4("u_ViewProjection", m_Game->GetCamera()->GetOrthoProjMatrix());
		glBindVertexArray(m_SpriteVAO);

		for (SpriteComponent* sprite : m_Sprites)
			sprite->Draw(m_SpriteShader);

		glDisable(GL_BLEND);

		SDL_GL_SwapWindow(m_Window);
	}

	void Renderer::AddMeshRenderer(MeshRenderer* mesh)
	{
		m_Meshes.emplace_back(mesh);
	}

	void Renderer::RemoveMeshRenderer(MeshRenderer* mesh)
	{
		auto iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
		if (iter != m_Meshes.end())
			m_Meshes.erase(iter);
	}

	void Renderer::AddSpriteComponent(SpriteComponent* sprite)
	{
		int drawOrder = sprite->GetDrawOrder();
		auto iter = m_Sprites.begin();
		
		for (; iter != m_Sprites.end(); iter++)
			if (drawOrder < (*iter)->GetDrawOrder())
				break;

		m_Sprites.insert(iter, sprite);
	}

	void Renderer::RemoveSpriteComponent(SpriteComponent* sprite)
	{
		auto iter = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
		if (iter != m_Sprites.end())
			m_Sprites.erase(iter);
	}

	Texture* Renderer::GetTexture(const std::string filename, Texture::TextureType type)
	{
		auto iter = m_Textures.find(filename);
		if (iter != m_Textures.end())
			return iter->second;

		Texture* texture = new Texture();

		if (!texture->Load(filename, type))
		{
			delete texture;
			texture = nullptr;
		}
		else
			m_Textures.emplace(filename, texture);

		return texture;
	}

	Model* Renderer::GetModel(const std::string filename)
	{
		auto iter = m_Models.find(filename);
		if (iter != m_Models.end())
			return iter->second;

		Model* model = new Model(filename, m_Game);
		m_Models.emplace(filename, model);

		return model;
	}

	void Renderer::LoadShaders()
	{
		m_MeshShader = new Shader("Assets/Shaders/Phong.vert", "Assets/Shaders/Phong.frag");
		m_SpriteShader = new Shader("Assets/Shaders/Sprite.vert", "Assets/Shaders/Sprite.frag");
	}
	
	void Renderer::InitSpriteVerts()
	{
		float vertices[] = {
			-0.5f, 0.5f, 0.f,	 0.f, 0.f, 0.0f,	 0.f, 0.f, // top left
			 0.5f, 0.5f, 0.f,	 0.f, 0.f, 0.0f,	 1.f, 0.f, // top right
			 0.5f,-0.5f, 0.f,	 0.f, 0.f, 0.0f,	 1.f, 1.f, // bottom right
			-0.5f,-0.5f, 0.f,	 0.f, 0.f, 0.0f,	 0.f, 1.f  // bottom left
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glGenVertexArrays(1, &m_SpriteVAO);
		glGenBuffers(1, &m_SpriteVBO);
		glGenBuffers(1, &m_SpriteEBO);

		glBindVertexArray(m_SpriteVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_SpriteVBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SpriteEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));

		glBindVertexArray(0);
	}
}