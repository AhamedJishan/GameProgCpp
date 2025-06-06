#pragma once

#include <vector>

namespace jLab
{
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	class Game
	{
	public:
		Game();

		bool Init();
		void Run();
		void Shutdown();

		class Renderer* GetRenderer() const { return m_Renderer; }
		class InputSystem* GetInputSystem() const { return m_InputSystem; }
		class AudioSystem* GetAudioSystem() const { return m_AudioSystem; }

		void AddActor(class Actor* actor);
		void RemoveActor(class Actor* actor);

		// Game specific code
		void HandleKeyPress(struct InputState& inputState);

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

		bool m_IsRunning;
		int m_TicksCount;

		bool m_UpdatingActors;
		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;

		// Game specifi members;
		class FollowActor* m_FollowActor;
		class FPSActor* m_FPSActor;
		class OrbitActor* m_OrbitActor;
		class SplineActor* m_SplineActor;
	};
}