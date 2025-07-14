#pragma once

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>

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

	private:
		class Game* m_Game;

		int m_ScreenWidth;
		int m_ScreenHeight;

		SDL_Window* m_Window;
		SDL_GLContext m_Context;
	};
}