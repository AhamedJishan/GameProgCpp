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

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		bool m_IsRunning;
		Uint32 m_TicksCount;

		class Renderer* m_Renderer;
		class InputSystem* m_InputSystem;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
	};
}