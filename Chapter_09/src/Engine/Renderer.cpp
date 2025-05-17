#include "Renderer.h"

#include "Game.h"
#include "Model.h"
#include "Actor.h"
#include "Components/MeshRenderer.h"
#include "Components/CameraComponent.h"
#include "Shader.h"

namespace jLab
{
	Renderer::Renderer(Game* game)
	{
		m_Game = game;
		m_Context = nullptr;
		m_Window = nullptr;
		m_MeshShader = nullptr;
	}
	
	bool Renderer::Init(int screenWidth, int screenHeight)
	{
		m_Window = SDL_CreateWindow("Chapter_09: Cameras", 200, 40, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
		if (!m_Window)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Window: %s", SDL_GetError());
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
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		m_Context = SDL_GL_CreateContext(m_Window);
		if (!m_Context)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create context: %s", SDL_GetError());
			return false;
		}

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise GLEW");
			return false;
		}
		glGetError();
		glViewport(0, 0, screenWidth, screenHeight);

		m_MeshShader = new Shader("Assets/Shaders/phong.vert", "Assets/Shaders/phong.frag");

		m_Projection = glm::perspective(glm::radians(80.0f), ((float)screenWidth / (float)screenHeight), 0.01f, 1000.0f);
		m_View = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

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

		// TODO: render the scene
		glEnable(GL_DEPTH_TEST);
		
		SetShaderUniforms();
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

	void Renderer::SetShaderUniforms()
	{
		glm::mat4 viewProjectionMatrix = m_Projection * m_View;

		// Extract rotation matrix (upper-left 3x3)
		glm::mat3 R = glm::mat3(m_View);
		// Extract translation vector (4th column)
		glm::vec3 t = glm::vec3(m_View[3]);
		// Calculate camera position
		glm::vec3 cameraPos = -glm::transpose(R) * t;

		m_MeshShader->SetActive();
		m_MeshShader->SetMat4("u_ViewProjection", viewProjectionMatrix);
		m_MeshShader->SetVec3("u_CameraPos", cameraPos);
		m_MeshShader->SetVec3("u_LightColor", glm::vec3(1.0f));
		m_MeshShader->SetVec3("u_LightDir", glm::vec3(1, -0.5f, -1));
		m_MeshShader->SetVec3("u_AmbientColor", glm::vec3(0.3f, 0.3f, 0.3f));
	}
}