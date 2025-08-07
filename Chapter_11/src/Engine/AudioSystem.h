#pragma once

#include <unordered_map>
#include <string>
#include <glm/mat4x4.hpp>
#include "SoundEvent.h"

// Forward declaration to avoid including FMOD headers
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bus;
		class Bank;
		class System;
		class EventInstance;
		class EventDescription;
	}
}

namespace jLab
{
	class AudioSystem
	{
	public:
		AudioSystem(class Game* game);
		~AudioSystem();

		bool Init();
		void Shutdown();

		void LoadBank(const std::string& name);
		void UnloadBank(const std::string& name);
		void UnloadAllBanks();

		void Update();

		SoundEvent PlayEvent(const std::string& name);

		void SetListener(const glm::mat4& viewMatrix);

		void SetBusVolume(const std::string& name, float volume);
		void SetBusPaused(const std::string& name, bool value);
		float GetBusVolume(const std::string& name) const;
		bool GetBusPaused(const std::string& name) const;

	protected:
		friend class SoundEvent;
		FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

	private:
		static unsigned int s_NextId;

		class Game* m_Game;
		class FMOD::Studio::System* m_System;

		std::unordered_map<std::string, FMOD::Studio::Bank*> m_Banks;
		std::unordered_map<std::string, FMOD::Studio::Bus*> m_Buses;
		std::unordered_map<std::string, FMOD::Studio::EventDescription*> m_Events;
		std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> m_EventInstances;
	};
}