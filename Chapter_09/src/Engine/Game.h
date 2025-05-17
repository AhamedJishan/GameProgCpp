#pragma once

#include <vector>

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
		class InputSystem* GetInputSystem() const { return m_InputSystem; }

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		class Renderer* m_Renderer;
		class InputSystem* m_InputSystem;

		bool m_IsRunning;
		int m_TicksCount;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
	};
}