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

		class Texture* GetTexture(const std::string& filename, Texture::TextureType textureType);
		class Model* GetModel(const std::string& filename);

		void AddMeshRenderer(class MeshRenderer* mesh);
		void RemoveMeshRenderer(class MeshRenderer* mesh);

		void SetViewMatrix(const glm::mat4 view) { m_View = view; }

		glm::vec3 ScreenToWorldPos(const glm::vec3& screenPosition);
		void ScreenToWorldDir(glm::vec3& outStart, glm::vec3& outDirection);

	private:
		void SetShaderUniforms();

	private:
		class Game* m_Game;

		int m_ScreenWidth;
		int m_ScreenHeight;

		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		std::unordered_map<std::string, Texture*> m_Textures;
		std::unordered_map<std::string, class Model*> m_Models;

		std::vector<class MeshRenderer*> m_Meshes;

		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_Ortho;

		class Shader* m_MeshShader;
	};
}