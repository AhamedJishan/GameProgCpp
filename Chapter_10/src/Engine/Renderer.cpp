#include "Renderer.h"

#include "Game.h"
#include "Model.h"
#include "Component/MeshRenderer.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace jLab
{
	Renderer::Renderer(Game* game)
		:m_Game(game)
	{
		m_MeshShader = nullptr;
	}
	
	Renderer::~Renderer()
	{
	}
	
	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		m_ScreenHeight = screenHeight;
		m_ScreenWidth = screenWidth;

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

		m_Window = SDL_CreateWindow("Chapter_10: Collisions", 10, 30, m_ScreenWidth, m_ScreenHeight, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window: %s", SDL_GetError());
			return false;
		}

		m_Context = SDL_GL_CreateContext(m_Window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialize GLEW");
			return false;
		}
		glGetError();

		m_MeshShader = new Shader("Assets/Shaders/Phong.vert", "Assets/Shaders/Phong.frag");
		m_Projection = glm::perspective(glm::radians(80.0f), ((float)m_ScreenWidth / (float)m_ScreenHeight), 0.1f, 1000.0f);
		m_View = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		m_Ortho = glm::ortho(-m_ScreenWidth / 2.0f, m_ScreenWidth / 2.0f, -m_ScreenHeight / 2.0f, m_ScreenHeight / 2.0f);

		return true;
	}
	
	void Renderer::Shutdown()
	{
		SDL_GL_DeleteContext(m_Context);
		SDL_DestroyWindow(m_Window);
	}
	
	void Renderer::Draw()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		SetShaderUniforms();
		for (MeshRenderer* mesh : m_Meshes)
			mesh->Draw(m_MeshShader);

		glDisable(GL_DEPTH_TEST);

		SDL_GL_SwapWindow(m_Window);
	}
	Texture* Renderer::GetTexture(const std::string& filename, Texture::TextureType textureType)
	{
		auto iter = m_Textures.find(filename);
		if (iter != m_Textures.end())
			return iter->second;

		Texture* texture = new Texture();
		
		if (!texture->Load(filename, textureType))
		{
			delete texture;
			texture = nullptr;
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture '%s'", filename.c_str());
		}
		else
			m_Textures.emplace(filename, texture);

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

	void Renderer::SetShaderUniforms()
	{
		glm::mat4 viewProjectionMatrix = m_Projection * m_View;

		glm::mat3 R = glm::mat3(m_View);
		glm::vec3 T = glm::vec3(m_View[3]);
		glm::vec3 cameraPos = -glm::transpose(R) * T;

		m_MeshShader->SetActive();
		m_MeshShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
		m_MeshShader->SetVec3("u_CameraPos", cameraPos);
		m_MeshShader->SetVec3("u_LightColor", glm::vec3(1.0f));
		m_MeshShader->SetVec3("u_LightDir", glm::vec3(1, -0.5f, -1));
		m_MeshShader->SetVec3("u_AmbientColor", glm::vec3(0.3f, 0.3f, 0.3f));
	}
}