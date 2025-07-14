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
		void Shutdown();

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		class Renderer* m_Renderer;

		bool m_IsRunning;
		Uint32 m_TicksCount;
	};
}