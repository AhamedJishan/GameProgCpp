#pragma once

#include <unordered_map>
#include <vector>
#include <string>

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

		void LoadText(const std::string& filename);
		const std::string& GetText(const std::string& key);

		std::vector<class WallActor*>& GetWallPlanes() { return mWallPlanes; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		GameState mGameState;
		unsigned int mTicksCount;

		class Renderer* mRenderer;
		class InputSystem* mInputSystem;
		class PhysWorld* mPhysWorld;

		bool mUpdatingActors;
		std::vector<class Actor*> mActors;
		std::vector<class Actor*> mPendingActors;

		std::unordered_map<std::string, std::string> mTexts;

		// Game Specific
		std::vector<class WallActor*> mWallPlanes;
	};
}