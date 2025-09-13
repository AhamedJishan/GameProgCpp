#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace jLab
{
	class Game
	{
	public:
		enum GameState
		{
			E_Gameplay,
			E_Paused,
			E_Quit
		};

		Game();

		bool Init();
		void Run();
		void Shutdown();

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		void LoadText(const std::string& filename);
		const std::string& GetText(const std::string& key);

		class Skeleton* GetSkeleton(const std::string& filename);

		class Renderer* GetRenderer() const { return m_Renderer; }
		class AudioSystem* GetAudioSystem() const { return m_AudioSystem; }
		class InputSystem* GetInputSystem() const { return m_InputSystem; }
		class PhysWorld* GetPhysWorld() const { return m_PhysWorld; }
		class Font* GetFont(const std::string& filename);

		void SetState(GameState state) { m_GameState = state; }
		GameState GetState() const { return m_GameState; }

		void PushUI(class UIScreen* screen);
		std::vector<class UIScreen*>& GetUIStack() { return m_UIStack; }

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
		class AudioSystem* m_AudioSystem;
		class PhysWorld* m_PhysWorld;

		GameState m_GameState;
		Uint32 m_TicksCount;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
		std::vector<class UIScreen*> m_UIStack;

		std::unordered_map<std::string, class Font*> m_Fonts;
		std::unordered_map<std::string, std::string> m_Texts;
		std::unordered_map<std::string, class Skeleton*> m_Skeletons;

		// GAME SPECIFIC
		std::vector<class WallActor*> m_WallPlanes;
	};
}