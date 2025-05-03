#pragma once

namespace jLab
{
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;

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
		class InputSystem* m_InputSystem;
		class Renderer* m_Renderer;

		bool m_IsRunning;
		int m_TicksCount;
	};
}