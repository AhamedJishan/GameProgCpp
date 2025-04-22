#pragma once

#include <SDL/SDL.h>
#include <vector>

namespace jLab
{
	class Game
	{
	public:
		Game();

		bool Init();
		void Run();
		void ShutDown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		void LoadData();
		void UnloadData();

		class Renderer* GetRenderer() { return m_Renderer; }

	private:
		void UpdateGame();
		void ProcessInput();
		void GenerateOutput();

	private:
		class Renderer* m_Renderer;
		class Camera* m_Camera;

		bool m_IsRunning;
		int m_TicksCount;

		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
		bool m_UpdatingActors;
	};
}