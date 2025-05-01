#include "AudioSystem.h"

#include <SDL/SDL.h>
#include <fmod_errors.h>
#include <glm/vec3.hpp>
#include "SoundEvent.h"


namespace jLab
{
	unsigned int AudioSystem::s_NextId = 0;

	FMOD_VECTOR VecToFmodVec(float x, float y, float z)
	{
		FMOD_VECTOR retVec;

		retVec.x = x;
		retVec.y = y;
		retVec.z = z;

		return retVec;
	}

	AudioSystem::AudioSystem(Game* game)
		:m_Game(game),
		m_System(nullptr),
		m_CoreSystem(nullptr)
	{
	}
	
	AudioSystem::~AudioSystem()
	{
	}
	
	bool AudioSystem::Init()
	{
		FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR);

		FMOD_RESULT result = FMOD::Studio::System::create(&m_System);
		if (result != FMOD_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Create FMOD: %s", FMOD_ErrorString(result));
			return false;
		}

		result = m_System->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Initialise FMOD: %s", FMOD_ErrorString(result));
			return false;
		}

		m_System->getCoreSystem(&m_CoreSystem);

		LoadBank("Assets/Audio/Master Bank.strings.bank");
		LoadBank("Assets/Audio/Master Bank.bank");

		return true;
	}
	
	void AudioSystem::Shutdown()
	{
	}
	
	void AudioSystem::Update(float deltaTime)
	{
		std::vector<unsigned int> stoppedEventIds;

		for (auto& iter : m_EventInstances)
		{
			FMOD::Studio::EventInstance* event = iter.second;
			FMOD_STUDIO_PLAYBACK_STATE state;

			event->getPlaybackState(&state);

			if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				event->release();
				stoppedEventIds.emplace_back(iter.first);
			}
		}

		for (unsigned int id : stoppedEventIds)
			m_EventInstances.erase(id);

		m_System->update();
	}
	
	void AudioSystem::LoadBank(const std::string name)
	{
		if (m_Banks.find(name) != m_Banks.end())
			return;

		FMOD::Studio::Bank* bank = nullptr;
		FMOD_RESULT result = m_System->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);

		const int maxPathLength = 512;

		if (result == FMOD_OK)
		{
			m_Banks.emplace(name, bank);
			bank->loadSampleData();

			// Fetching events
			int numEvents = 0;
			bank->getEventCount(&numEvents);
			if (numEvents > 0)
			{
				std::vector<FMOD::Studio::EventDescription*> events(numEvents);
				bank->getEventList(events.data(), numEvents, &numEvents);

				char eventName[maxPathLength];
				for (int i = 0; i < numEvents; i++)
				{
					FMOD::Studio::EventDescription* event = events[i];
					event->getPath(eventName, maxPathLength, nullptr);
					m_Events.emplace(eventName, event);
				}
			}

			// Fetching Buses
			int numBuses = 0;
			bank->getBusCount(&numBuses);
			if (numBuses > 0)
			{
				std::vector<FMOD::Studio::Bus*> buses(numBuses);
				bank->getBusList(buses.data(), numBuses, &numBuses);

				char busName[maxPathLength];
				for (int i = 0; i < numBuses; i++)
				{
					FMOD::Studio::Bus* bus = buses[i];
					bus->getPath(busName, maxPathLength, nullptr);
					m_Buses.emplace(busName, bus);
				}
			}

		}
		else
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to Load Bank '%s': %s", name.c_str(), FMOD_ErrorString(result));
		}
	}

	void AudioSystem::UnloadBank(const std::string name)
	{
		auto iter = m_Banks.find(name);
		if (iter == m_Banks.end())
			return;

		FMOD::Studio::Bank* bank = iter->second;

		const int maxPathLength = 512;

		int numEvents = 0;
		bank->getEventCount(&numEvents);

		if (numEvents > 0)
		{
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);

			char eventName[maxPathLength];

			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* event = events[i];
				event->getPath(eventName, maxPathLength, nullptr);

				auto eventi = m_Events.find(name);
				if (eventi != m_Events.end())
					m_Events.erase(eventi);
			}
		}

		bank->unloadSampleData();
		bank->unload();
		m_Banks.erase(name);
	}

	void AudioSystem::UnloadAllBanks()
	{
		for (auto& iter : m_Banks)
		{
			iter.second->unloadSampleData();
			iter.second->unload();
		}
		m_Banks.clear();

		m_Events.clear();
	}

	SoundEvent AudioSystem::PlayEvent(const std::string& name)
	{
		unsigned int eventId = 0;

		auto iter = m_Events.find(name);
		if (iter != m_Events.end())
		{
			FMOD::Studio::EventInstance* event = nullptr;
			iter->second->createInstance(&event);

			if (event)
			{
				event->start();

				eventId = ++s_NextId;
				m_EventInstances.emplace(eventId, event);
			}

			return SoundEvent(this, eventId);
		}
		
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Event '%s' is not a part of any loaded audio banks", name.c_str());
		return SoundEvent();
	}

	void AudioSystem::SetListener(glm::mat4& viewMatrix)
	{
		glm::mat4 invViewMat = glm::inverse(viewMatrix);

		FMOD_3D_ATTRIBUTES listener;

		listener.position = VecToFmodVec(invViewMat[3][0], invViewMat[3][1], invViewMat[3][2]);
		listener.forward =	VecToFmodVec(invViewMat[2][0], invViewMat[2][1], invViewMat[2][2]);
		listener.up =		VecToFmodVec(invViewMat[1][0], invViewMat[1][1], invViewMat[1][2]);

		listener.velocity = { 0, 0, 0 };

		m_System->setListenerAttributes(0, &listener);
	}

	float AudioSystem::GetBusVolume(const std::string& name)
	{
		float volume = 0.0f;
		auto iter = m_Buses.find(name);

		if (iter != m_Buses.end())
			iter->second->getVolume(&volume);
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bus '%s' is not a part of any loaded buses", name.c_str());

		return volume;
	}

	bool AudioSystem::GetBusPaused(const std::string& name)
	{
		bool paused = false;
		auto iter = m_Buses.find(name);

		if (iter != m_Buses.end())
			iter->second->getPaused(&paused);
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bus '%s' is not a part of any loaded buses", name.c_str());

		return paused;
	}

	void AudioSystem::SetBusVolume(const std::string& name, float volume)
	{
		auto iter = m_Buses.find(name);

		if (iter != m_Buses.end())
			iter->second->setVolume(volume);
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bus '%s' is not a part of any loaded buses", name.c_str());
	}

	void AudioSystem::SetBusPaused(const std::string& name, bool pause)
	{
		auto iter = m_Buses.find(name);

		if (iter != m_Buses.end())
			iter->second->setPaused(pause);
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Bus '%s' is not a part of any loaded buses", name.c_str());
	}

	FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
	{
		FMOD::Studio::EventInstance* event = nullptr;

		auto iter = m_EventInstances.find(id);
		if (iter != m_EventInstances.end())
			event = iter->second;

		return event;
	}

}