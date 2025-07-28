#include "AudioSystem.h"

#include <SDL/SDL.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

namespace
{
	FMOD_VECTOR VecToFMOD(glm::vec3& vec)
	{
		FMOD_VECTOR outVec;
		outVec.x = vec.x;
		outVec.y = vec.y;
		outVec.z = vec.z;
		return outVec;
	}
}

namespace jLab
{
	unsigned int AudioSystem::s_NextId = 0;

	AudioSystem::AudioSystem(Game* game)
		:m_Game(game)
		,m_System(nullptr)
	{
	}
	
	AudioSystem::~AudioSystem()
	{
	}
	
	bool AudioSystem::Init()
	{
		FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR);

		FMOD_RESULT result;
		result = FMOD::Studio::System::create(&m_System);
		if (result != FMOD_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create FMOD system: %s", FMOD_ErrorString(result));
			return false;
		}

		result = m_System->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
			return false;
		}

		LoadBank("Assets/Audio/Master Bank.strings.bank");
		LoadBank("Assets/Audio/Master Bank.bank");

		return true;
	}
	
	void AudioSystem::Shutdown()
	{
		UnloadAllBanks();
		if (m_System)
			m_System->release();
	}
	
	void AudioSystem::LoadBank(const std::string& name)
	{
		if (m_Banks.find(name) != m_Banks.end())
		{
			SDL_Log("FMOD Bank '%s' is already loaded!", name);
			return;
		}

		FMOD::Studio::Bank* bank = nullptr;
		FMOD_RESULT result = m_System->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
		
		const int maxPathLength = 512;
		if (result == FMOD_OK)
		{
			m_Banks.emplace(name, bank);
			bank->loadSampleData();

			// Load Events
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

			//Load Buses
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
			SDL_Log("Failed to load FMOD Bank '%s'!", name);
	}
	
	void AudioSystem::UnloadBank(const std::string& name)
	{
		auto iter = m_Banks.find(name);
		if (iter == m_Banks.end())
		{
			SDL_Log("FMOD Bank '%s' doesn't exist!", name);
			return;
		}

		FMOD::Studio::Bank* bank = iter->second;
		const int maxPathLength = 512;

		int numEvents = 0;
		bank->getEventCount(&numEvents);
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		char eventName[maxPathLength];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* event = events[i];
			event->getPath(eventName, maxPathLength, nullptr);

			auto eventIter = m_Events.find(eventName);
			if (eventIter != m_Events.end())
				m_Events.erase(eventIter);
		}

		int numBuses= 0;
		bank->getBusCount(&numBuses);
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		char busName[maxPathLength];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			bus->getPath(busName, maxPathLength, nullptr);

			auto busIter = m_Buses.find(busName);
			if (busIter != m_Buses.end())
				m_Buses.erase(busIter);
		}

		bank->unloadSampleData();
		bank->unload();
		m_Banks.erase(iter);
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
		m_Buses.clear();
	}
	
	void AudioSystem::Update()
	{
		// Find any stopped event instances
		std::vector<unsigned int> done;
		for (auto& iter : m_EventInstances)
		{
			FMOD::Studio::EventInstance* event = iter.second;
			FMOD_STUDIO_PLAYBACK_STATE state;
			event->getPlaybackState(&state);
			if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				event->release();
				done.emplace_back(iter.first);
			}
		}
		// Remove done event instances
		for (auto id : done)
			m_EventInstances.erase(id);

		// Update FMOD
		m_System->update();
	}

	SoundEvent AudioSystem::PlayEvent(const std::string& name)
	{
		int id = 0;
		auto iter = m_Events.find(name);
		if (iter != m_Events.end())
		{
			FMOD::Studio::EventInstance* event = nullptr;
			iter->second->createInstance(&event);
			if (event)
			{
				event->start();
				id = ++s_NextId;
				m_EventInstances.emplace(id, event);
			}
		}
		return SoundEvent(this, id);
	}

	void AudioSystem::SetListener(const glm::mat4& viewMatrix)
	{
		glm::mat4 inView = glm::inverse(viewMatrix);
		glm::vec3 pos = inView[3];
		glm::vec3 forward = -inView[2];
		glm::vec3 up = inView[1];

		FMOD_3D_ATTRIBUTES attrib;
		attrib.position = VecToFMOD(pos);
		attrib.forward = VecToFMOD(forward);
		attrib.up = VecToFMOD(up);
		attrib.velocity = VecToFMOD(glm::vec3(0));

		m_System->setListenerAttributes(0, &attrib);
	}

	void AudioSystem::SetBusVolume(const std::string& name, float volume)
	{
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			FMOD::Studio::Bus* bus = iter->second;
			bus->setVolume(volume);
		}
	}

	void AudioSystem::SetBusPaused(const std::string& name, bool value)
	{
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			FMOD::Studio::Bus* bus = iter->second;
			bus->setPaused(value);
		}
	}

	float AudioSystem::GetBusVolume(const std::string& name) const
	{
		float retValue = 0.0f;
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			FMOD::Studio::Bus* bus = iter->second;
			bus->getVolume(&retValue);
		}
		return retValue;
	}

	bool AudioSystem::GetBusPaused(const std::string& name) const
	{
		bool retValue = false;
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			FMOD::Studio::Bus* bus = iter->second;
			bus->getPaused(&retValue);
		}
		return retValue;
	}

	FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
	{
		FMOD::Studio::EventInstance* event = nullptr;

		auto iter = m_EventInstances.find(id);
		if (iter != m_EventInstances.end())
			event = iter->second;

		return nullptr;
	}
}