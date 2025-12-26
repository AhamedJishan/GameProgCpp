#pragma once

#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>

namespace FMOD
{
	namespace Studio
	{
		class Bank;
		class Bus;
		class EventDescription;
		class EventInstance;
		class System;
	}
}

namespace jLab
{
	class AudioSystem
	{
	public:
		AudioSystem(class Game* game);

		bool Init();
		void Shutdown();

		void Update(float deltaTime);

		void LoadBank(const std::string& name);
		void UnloadBank(const std::string& name);
		void UnloadAllBanks();

		// TODO: void PlayEvent();

		void SetListener(glm::mat4 view);

		float GetBusVolume(const std::string& name) const;
		bool GetBusPaused(const std::string& name) const;
		void SetBusVolume(const std::string& name, float volume);
		void SetBusPaused(const std::string& name, bool pause);

	private:
		static unsigned int sNextId;

		class Game* mGame;
		std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
		std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
		std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEventDescriptions;
		std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
		FMOD::Studio::System* mSystem;
	};
}