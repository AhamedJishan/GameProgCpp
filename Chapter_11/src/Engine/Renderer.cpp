#include "Renderer.h"

#include "Game.h"
#include "Model.h"
#include "Component/MeshRenderer.h"
#include "Component/SpriteRenderer.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

namespace jLab
{
	Renderer::Renderer(Game* game)
		:m_Game(game)
	{
		m_MeshShader = nullptr;
		m_SpriteShader = nullptr;
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
		m_SpriteShader = new Shader("Assets/Shaders/Sprite.vert", "Assets/Shaders/Sprite.frag");;
		m_Projection = glm::perspective(glm::radians(80.0f), ((float)m_ScreenWidth / (float)m_ScreenHeight), 0.1f, 1000.0f);
		m_View = glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		m_Ortho = glm::ortho(-m_ScreenWidth / 2.0f, m_ScreenWidth / 2.0f, -m_ScreenHeight / 2.0f, m_ScreenHeight / 2.0f);

		CreateSpriteVerts();

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

		SetShaderUniforms();
		for (MeshRenderer* mesh : m_Meshes)
			mesh->Draw(m_MeshShader);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		UseSpriteVerts();
		for (SpriteRenderer* sprite : m_Sprites)
			sprite->Draw(m_SpriteShader);

		glDisable(GL_BLEND);

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

	void Renderer::AddSpriteRenderer(SpriteRenderer* sprite)
	{
		int drawOrder = sprite->GetDrawOrder();
		auto iter = m_Sprites.begin();
		for (; iter != m_Sprites.end(); iter++)
			if (drawOrder < (*iter)->GetDrawOrder())
				break;
		m_Sprites.insert(iter, sprite);
	}

	void Renderer::RemoveSpriteRenderer(SpriteRenderer* sprite)
	{
		auto iter = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
		if (iter != m_Sprites.end())
			m_Sprites.erase(iter);
	}

	glm::vec3 Renderer::ScreenToWorldPos(const glm::vec3& screenPosition)
	{
		glm::vec3 ndc = screenPosition;
		ndc.x /= m_ScreenWidth * 0.5f;
		ndc.y /= m_ScreenHeight * 0.5f;

		glm::mat4 unprojection = glm::inverse(m_Projection * m_View);
		glm::vec4 pos = unprojection * glm::vec4(ndc, 1.0f);
		pos /= pos.w;

		return glm::vec3(pos);
	}

	void Renderer::ScreenToWorldDir(glm::vec3& outStart, glm::vec3& outDirection)
	{
		glm::vec3 ndcPos = glm::vec3(0);
		outStart = ScreenToWorldPos(ndcPos);
		ndcPos.z = 0.9f;
		glm::vec3 end = ScreenToWorldPos(ndcPos);
		outDirection = glm::normalize(end - outStart);
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

	void Renderer::CreateSpriteVerts()
	{
		float vertices[] = {
			-0.5f,  0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
			 0.5f,  0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
			 0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
			-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SpriteEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Renderer::UseSpriteVerts()
	{
		glBindVertexArray(m_SpriteVAO);
		m_SpriteShader->SetActive();
		m_SpriteShader->SetMat4("u_ViewProjection", m_Ortho);
	}
}