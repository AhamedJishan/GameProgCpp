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
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void InitSpriteVerts();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		class VertexArray* m_SpriteVerts;
		
		bool m_IsRunning;
		int m_TicksCount;
	};
}