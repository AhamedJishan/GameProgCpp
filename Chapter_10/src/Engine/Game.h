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

		class Renderer* GetRenderer() const { return m_Renderer; }
		class PhysWorld* GetPhysWorld() const { return m_PhysWorld; }

		std::vector<class WallActor*>& GetWallPlanes() { return m_WallPlanes; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		class Renderer* m_Renderer;
		class InputSystem* m_InputSystem;
		class PhysWorld* m_PhysWorld;

		bool m_IsRunning;
		Uint32 m_TicksCount;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;

		// GAME SPECIFIC
		std::vector<class WallActor*> m_WallPlanes;
	};
}