#pragma once

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include "Texture.h"

namespace jLab
{
	class Renderer
	{
	public:
		Renderer(class Game* game);
		~Renderer();

		bool Init(int screenWidth, int screenHeight);
		void Shutdown();

		void Draw();

		class Texture* GetTexture(const std::string& filename, Texture::TextureType textureType = Texture::E_Diffuse);
		class Model* GetModel(const std::string& filename);

		void AddMeshRenderer(class MeshRenderer* mesh);
		void RemoveMeshRenderer(class MeshRenderer* mesh);
		void AddSpriteRenderer(class SpriteRenderer* sprite);
		void RemoveSpriteRenderer(class SpriteRenderer* sprite);

		void SetViewMatrix(const glm::mat4 view) { m_View = view; }

		glm::vec3 ScreenToWorldPos(const glm::vec3& screenPosition);
		void ScreenToWorldDir(glm::vec3& outStart, glm::vec3& outDirection);

		int GetWidth() const { return m_ScreenWidth; }
		int GetHeight() const { return m_ScreenHeight; }
		SDL_Window* GetWindow() const { return m_Window; }

	private:
		void SetShaderUniforms();
		void CreateSpriteVerts();
		void UseSpriteVerts();

	private:
		class Game* m_Game;

		int m_ScreenWidth;
		int m_ScreenHeight;

		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Ortho;

		std::unordered_map<std::string, Texture*> m_Textures;
		std::unordered_map<std::string, class Model*> m_Models;

		std::vector<class MeshRenderer*> m_Meshes;
		std::vector<class SpriteRenderer*> m_Sprites;

		class Shader* m_MeshShader;
		class Shader* m_SpriteShader;
		unsigned int m_SpriteVAO, m_SpriteVBO, m_SpriteEBO;
	};
}