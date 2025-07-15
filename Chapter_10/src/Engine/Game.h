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
		void Shutdown();

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

		bool m_IsRunning;
		Uint32 m_TicksCount;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
	};
}