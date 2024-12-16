#pragma once

#include <SDL.h>

#include <vector>
#include "Actor.h"

namespace ch2
{
	class Game
	{
	public:
		Game();
		~Game();

		bool Init();
		void RunLoop();
		void Shutdown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		SDL_Window* m_Window;
		bool m_IsRunning;
		Uint32 m_TicksCount;

		std::vector<Actor*> m_Actors;
		std::vector<Actor*> m_PendingActors;
		bool m_UpdatingActor;
	};

}