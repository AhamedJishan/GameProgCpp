#pragma once

#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>

namespace jLab
{
	class Game
	{
	public:
		Game();
		~Game();

		bool Init();
		void Run();
		void Shutdown();

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		bool m_IsRunning;
		Uint32 m_TicksCount;

		class Renderer* m_Renderer;
	};
}