#pragma once

#include <SDL/SDL.h>

namespace jLab
{
	class Game
	{
	public:
		Game();

		bool Init();
		void Run();
		void ShutDown();

	private:
		void UpdateGame();
		void ProcessInput();
		void GenerateOutput();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		bool m_IsRunning;
		int m_TicksCount;
	};
}