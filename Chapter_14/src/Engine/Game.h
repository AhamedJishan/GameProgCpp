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

		GameState GetState() const { return mGameState; }
		void SetState(GameState state) { mGameState = state; }

		void PushUI(class UIScreen* screen);
		std::vector<class UIScreen*> GetUIStack() const { return mUIStack; }

		class Renderer*		GetRenderer()		const { return mRenderer; }
		class AudioSystem*	GetAudioSystem()	const { return mAudioSystem; }
		class InputSystem*	GetInputSystem()	const { return mInputSystem; }
		class PhysWorld*	GetPhysWorld()		const { return mPhysWorld; }
		class Skeleton*		GetSkeleton	(const std::string& filename);
		class Animation*	GetAnimation(const std::string& filename, class Skeleton* skeleton);
		class Font*			GetFont		(const std::string& filename);

		void LoadText(const std::string& filename);
		const std::string& GetText(const std::string& key);

		// Game specific
		std::vector<class WallActor*>& GetWallPlanes() { return mWallPlanes; }
		std::vector<class Actor*>& GetTargets() { return mTargets; }
		void AddTarget(class Actor* target) { mTargets.emplace_back(target); }

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
		class AudioSystem* mAudioSystem;
		class PhysWorld* mPhysWorld;

		bool mUpdatingActors;
		std::vector<class Actor*> mActors;
		std::vector<class Actor*> mPendingActors;
		std::vector<class UIScreen*> mUIStack;

		std::unordered_map<std::string, class Skeleton*> mSkeletons;
		std::unordered_map<std::string, class Animation*> mAnimations;
		std::unordered_map<std::string, class Font*> mFonts;
		std::unordered_map<std::string, std::string> mTexts;

		// Game Specific
		std::vector<class WallActor*> mWallPlanes;
		std::vector<class Actor*> mTargets;
	};
}