#pragma once

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

		class Renderer* GetRenderer() { return m_Renderer; }
		class AudioSystem* GetAudioSystem() { return m_AudioSystem; }
		class Camera* GetCamera() { return m_Camera; }

	private:
		void ProcessInput();
		void UpdateGame();
		void GenerateOutput();

		void LoadData();
		void UnloadData();

	private:
		class Renderer* m_Renderer;
		class AudioSystem* m_AudioSystem;
		class Camera* m_Camera;

		int m_TicksCount;
		bool m_IsRunning;

		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
		bool m_UpdatingActors;
	};
}