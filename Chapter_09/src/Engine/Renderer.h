#pragma once

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <glm/mat4x4.hpp>
#include "Texture.h"

namespace jLab
{
	class Renderer
	{
	public:
		Renderer(class Game* game);

		bool Init(int screenWidth, int screenHeight);
		void Shutdown();

		void Draw();

		void AddMeshRenderer(class MeshRenderer* mesh);
		void RemoveMeshRenderer(class MeshRenderer* mesh);

		void AddSprite(class SpriteComponent* sprite);
		void RemoveSprite(class SpriteComponent* sprite);

		class Texture* GetTexture(const std::string filename, Texture::TextureType type = Texture::E_Diffuse);
		class Model* GetModel(const std::string filename);

		glm::vec3 ScreenToWorldPos(const glm::vec3& screenPosition);
		void ScreenToWorldDir(glm::vec3& outStart, glm::vec3& outDir);

		void SetViewMatrix(const glm::mat4 view) { m_View = view; }

	private:
		void SetShaderUniforms();
		void InitSpriteVerts();
		void UnloadData();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		class Game* m_Game;

		class Shader* m_SpriteShader;
		unsigned int m_SpriteVAO, m_SpriteVBO, m_SpriteEBO;
		class Shader* m_MeshShader;
		std::vector<class MeshRenderer*> m_Meshes;
		std::vector<class SpriteComponent*> m_Sprites;

		std::unordered_map<std::string, Texture*> m_Textures;
		std::unordered_map<std::string, class Model*> m_Models;

		glm::mat4 m_Ortho;
		glm::mat4 m_View;
		glm::mat4 m_Projection;
	};
}