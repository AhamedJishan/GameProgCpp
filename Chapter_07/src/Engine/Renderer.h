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

		int GetWidth() { return m_Width; }
		int GetHeight() { return m_Height; }

		class Texture* GetTexture(const std::string filename, Texture::TextureType type);

	private:
		class Game* m_Game;
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		int m_Width;
		int m_Height;

		std::unordered_map<std::string, class Texture*> m_Textures;
	};
}