#pragma once

#include <SDL/SDL.h>

namespace jLab
{
	class Game
	{
	public:
		Game();

		bool Init();
		void RunLoop();
		void Shutdown();

		// Actor related stuff
		// Sprites related stuff
		// Texture related stuff

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		Uint32			m_TicksCount;
		SDL_Renderer*	m_Renderer;
		SDL_Window*		m_Window;
		bool			m_IsRunning;
	};
}