#pragma once

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace ch3
{
	class Game
	{
	public:
		Game();

		bool init();
		void runLoop();
		void shutdown();

		SDL_Texture* getTexture(const char* filename);

	private:
		void processInput();
		void updateGame();
		void generateOutput();

	private:
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

		bool m_IsRunnning;
		Uint32 m_TicksCount;
	};
}