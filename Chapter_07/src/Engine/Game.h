#pragma once

namespace jLab
{
	class Game
	{
	public:
		Game();

		bool Init();
		void Run();
		void Shutdown();

		// TODO: Actor management

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		class Renderer* m_Renderer;

		int m_TicksCount;
		bool m_IsRunning;
	};
}