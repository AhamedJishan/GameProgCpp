#pragma once

#include <GLEW/GL/glew.h>
#include <SDL/SDL.h>

namespace jLab
{
	class Renderer
	{
	public:
		Renderer(class Game* game);

		bool Init(int screenWidth, int screenHeight);
		void Shutdown();

		void Draw();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		class Game* m_Game;

		int m_Width;
		int m_Height;
	};
}