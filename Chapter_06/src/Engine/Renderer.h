#pragma once

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
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
		void UnloadData();

		void Draw(class Camera* camera);

		void AddSpriteComponent(class SpriteComponent* sprite);
		void RemoveSpriteComponent(class SpriteComponent* sprite);

		void AddMeshComponent(class MeshComponent* mesh);
		void RemoveMeshComponent(class MeshComponent* mesh);

		class Texture* GetTexture(const std::string& filename, Texture::TextureType type);
		class Model* GetModel(const std::string& filename);

		int GetScreenWidth() { return m_Width; }
		int GetScreenHeight() { return m_Height; }

	private:
		int m_Width;
		int m_Height;

		SDL_Window* m_Window;
		SDL_GLContext m_Context;
		class Game* m_Game;

		class Shader* m_MeshShader;
		class Shader* m_SpriteShader;

		std::unordered_map<std::string, class Texture*> m_Textures;
		std::unordered_map<std::string, class Model*> m_Models;
		std::vector<class MeshComponent*> m_Meshes;
		std::vector<class SpriteComponent*> m_Sprites;
	};
}