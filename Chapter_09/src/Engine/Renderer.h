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

		bool Init(int screenWidth, int screenHeight);
		void Shutdown();

		void Draw();

		class Texture* GetTexture(const std::string filename, Texture::TextureType type = Texture::E_Diffuse);

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		class Game* m_Game;

		int m_Width;
		int m_Height;

		std::unordered_map<std::string, Texture*> m_Textures;
	};
}