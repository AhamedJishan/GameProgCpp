#pragma once

#include <SDL/SDL.h>
#include <GLEW/GL/glew.h>
#include <string>
#include <unordered_map>
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

		void AddMeshRenderer(class MeshRenderer* mesh);
		void RemoveMeshRenderer(class MeshRenderer* mesh);

		void AddSpriteComponent(class SpriteComponent* sprite);
		void RemoveSpriteComponent(class SpriteComponent* sprite);

		class Texture* GetTexture(const std::string filename, Texture::TextureType type);
		class Model* GetModel(const std::string filename);

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

	private:
		void LoadShaders();
		void InitSpriteVerts();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;
		class Game* m_Game;
		class Shader* m_MeshShader;
		class Shader* m_SpriteShader;

		unsigned int m_SpriteVAO;
		unsigned int m_SpriteVBO;
		unsigned int m_SpriteEBO;

		int m_Width;
		int m_Height;

		std::unordered_map<std::string, class Texture*> m_Textures;
		std::unordered_map<std::string, class Model*> m_Models;
		std::vector<class MeshRenderer*> m_Meshes;
		std::vector<class SpriteComponent*> m_Sprites;
	};
}