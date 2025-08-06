#pragma once

#include <SDL/SDL.h>
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

		class Texture* GetTexture(const std::string& filename, Texture::TextureType type =Texture::E_Diffuse);

	private:
		int m_ScreenWidth;
		int m_ScreenHeight;

		class Game* m_Game;

		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		std::unordered_map<std::string, Texture*> m_Textures;
	};
}