#include "Renderer.h"

#include <algorithm>
#include "Camera.h"
#include "Component/MeshComponent.h"
#include "Model.h"
#include "Shader.h"
#include "Component/SpriteComponent.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
		:m_Game(game),
		m_Context(NULL),
		m_Window(nullptr)
	{
	}
	
	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		m_Width = screenWidth;
		m_Height = screenHeight;

		m_Window = SDL_CreateWindow("Chapter_06: 3D OpenGl", 100, 30, m_Width, m_Height, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_Log("[ERROR] Failed to Create Window: %s", SDL_GetError());
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
			SDL_Log("ERROR: Failed to Initialise GLEW");
			return false;
		}
		glGetError();

		glViewport(0, 0, m_Width, m_Height);

		// Setup Shader
		m_MeshShader = new Shader("Assets/Shaders/BasicMesh.vert", "Assets/Shaders/BasicMesh.frag");
		m_SpriteShader = new Shader("Assets/Shaders/BasicSprite.vert", "Assets/Shaders/BasicSprite.frag");

		return true;
	}
	
	void Renderer::Shutdown()
	{
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
	}

	void Renderer::UnloadData()
	{
		delete m_SpriteShader;
		delete m_MeshShader;

		for (auto i : m_Textures)
			delete i.second;

		for (auto i : m_Models)
			delete i.second;
	}
	
	// --------------------DRAW---------------------------
	void Renderer::Draw(Camera* camera)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		m_MeshShader->SetActive();
		m_MeshShader->SetMat4("uViewProjection", camera->GetViewProjMatrix());

		for (MeshComponent* mesh : m_Meshes)
			mesh->Draw(m_MeshShader);

		glDisable(GL_DEPTH_TEST);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_SpriteShader->SetActive();
		m_SpriteShader->SetMat4("uViewProjection", camera->GetOrthoViewProjMatrix());

		for (SpriteComponent* sprite : m_Sprites)
			sprite->Draw(m_SpriteShader);

		glDisable(GL_BLEND);

		SDL_GL_SwapWindow(m_Window);
	}
	// ----------------------------------------------------

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
		m_Sprites.erase(iter);
	}

	void Renderer::AddMeshComponent(MeshComponent* mesh)
	{
		m_Meshes.emplace_back(mesh);
	}
	
	void Renderer::RemoveMeshComponent(MeshComponent* mesh)
	{
		auto iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
		if (iter != m_Meshes.end())
			m_Meshes.erase(iter);
	}
	
	Texture* Renderer::GetTexture(const std::string& filename, Texture::TextureType type)
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
	
	Model* Renderer::GetModel(const std::string& filename)
	{
		auto iter = m_Models.find(filename);
		if (iter != m_Models.end())
			return iter->second;

		Model* model = new Model(filename, m_Game);
		m_Models.emplace(filename, model);
		return model;
	}
}