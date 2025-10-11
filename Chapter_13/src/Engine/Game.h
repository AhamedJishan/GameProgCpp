#pragma once

#include <unordered_map>
#include <vector>

namespace jLab
{
	class Game
	{
	public:
		enum class GameState
		{
			Gameplay,
			Paused,
			Quit
		};

		Game();

		bool Init();
		void Run();
		void Shutdown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		class Renderer* GetRenderer() const { return mRenderer; }
		class InputSystem* GetInputSystem() const { return mInputSystem; }
		class PhysWorld* GetPhysWorld() const { return mPhysWorld; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		GameState mGameState;
		unsigned int mTicksCount;

		class Renderer* mRenderer;
		class InputSystem* mInputSystem;
		class PhysWorld* mPhysWorld;

		bool mUpdatingActors;
		std::vector<class Actor*> mActors;
		std::vector<class Actor*> mPendingActors;
	};
}