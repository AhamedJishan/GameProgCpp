#pragma once

#include <SDL/SDL.h>
#include <GLEW/GL/glew.h>

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
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		int m_Width;
		int m_Height;
	};
}