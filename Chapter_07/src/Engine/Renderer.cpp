#include "Renderer.h"

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>
#include "Game.h"
#include "Shader.h"
#include "Camera.h"
#include "Component/MeshRenderer.h"
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
		Shutdown();
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

		// TODO: Load Shaders and create spriteverts
		m_MeshShader = new Shader("Assets/Shaders/Phong.vert", "Assets/Shaders/Phong.frag");

		return true;
	}
	
	void Renderer::Shutdown()
	{
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
	}
	
	void Renderer::Draw()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		m_MeshShader->SetActive();
		m_MeshShader->SetMat4("u_ViewProjection", m_Game->GetCamera()->GetViewProjMatrix());
		m_MeshShader->SetVec3("u_CameraPos", m_Game->GetCamera()->GetPosition());
		m_MeshShader->SetVec3("u_LightColor", glm::vec3(1.0f));
		m_MeshShader->SetVec3("u_LightDir", glm::vec3(1, -0.5f, -1));
		m_MeshShader->SetVec3("u_AmbientColor", glm::vec3(0.2f, 0.2f, 0.3f));

		for (MeshRenderer* mesh : m_Meshes)
			mesh->Draw(m_MeshShader);


		glDisable(GL_DEPTH_TEST);

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
}