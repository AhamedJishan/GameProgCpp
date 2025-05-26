#pragma once

#include <unordered_map>
#include <string>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include "SoundEvent.h"

// Forward declaration for FMOD
namespace FMOD
{
	namespace Studio
	{
		class System;
		class Bank;
		class EventDescription;
		class EventInstance;
		class Bus;
	}
}

namespace jLab
{
	FMOD_VECTOR VecToFMOD(const glm::vec3& vec);

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

		SoundEvent PlayEvent(const std::string& name);

		void Update(float deltaTime);

		void SetListener(const glm::mat4& viewMatrix);

		void SetBusVolume(const std::string& name, const float value);
		void SetBusPaused(const std::string& name, const bool value);
		float GetBusVolume(const std::string& name) const;
		float GetBusPaused(const std::string& name) const;

	protected:
		friend class SoundEvent;
		FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

	private:
		class Game* m_Game;
		FMOD::Studio::System* m_System;
		static unsigned int s_NextId;

		std::unordered_map<std::string, FMOD::Studio::Bank*> m_Banks;
		std::unordered_map<std::string, FMOD::Studio::EventDescription*> m_Events;
		std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> m_EventInstances;
		std::unordered_map<std::string, FMOD::Studio::Bus*> m_Buses;
	};
}