#pragma once

#include <SDL.h>

namespace ch2
{
	class Game
	{
	public:
		Game();
		~Game();

		bool Init();
		void RunLoop();
		void Shutdown();

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		SDL_Window* m_Window;
		bool m_IsRunning;
		Uint32 m_TicksCount;
	};

}