#pragma once

#include <fmod_studio.hpp>
#include <unordered_map>
#include <vector>
#include <string>

namespace jLab
{
	class AudioSystem
	{
	public:
		AudioSystem(class Game* game);
		~AudioSystem();

		bool Init();
		void Shutdown();
		void Update(float deltaTime);

		void LoadBank(const std::string name);
		void UnloadBank(const std::string name);
		void UnloadAllBanks();

		class SoundEvent PlayEvent(const std::string& name);

	protected:
		friend class SoundEvent;
		FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

	private:
		static unsigned int s_NextId;

		class Game* m_Game;
		FMOD::Studio::System* m_System;
		FMOD::System* m_CoreSystem;

		std::unordered_map<std::string, FMOD::Studio::Bank*> m_Banks;
		std::unordered_map<std::string, FMOD::Studio::EventDescription*> m_Events;
		std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> m_EventInstances;
	};
}