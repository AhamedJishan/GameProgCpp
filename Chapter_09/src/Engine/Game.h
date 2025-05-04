#pragma once

namespace jLab
{
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	class Game
	{
	public:
		Game();

		bool Init();
		void Run();
		void Shutdown();

		class Renderer* GetRenderer() const { return m_Renderer; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		class Renderer* m_Renderer;
		bool m_IsRunning;
		int m_TicksCount;
	};
}