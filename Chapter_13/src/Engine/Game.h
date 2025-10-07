#pragma once

#include <unordered_map>
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

		class Renderer* GetRenderer() const { return mRenderer; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

	private:
		bool mIsRunning;

		class Renderer* mRenderer;
		class InputSystem* mInputSystem;

		bool mUpdatingActors;
		std::vector<class Actor*> mActors;
		std::vector<class Actor*> mPendingActors;
	};
}