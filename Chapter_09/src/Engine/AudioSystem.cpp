#include "AudioSystem.h"

#include <vector>
#include <SDL/SDL.h>

namespace jLab
{
	unsigned int AudioSystem::s_NextId = 0;

	FMOD_VECTOR VecToFMOD(const glm::vec3& vec)
	{
		FMOD_VECTOR ret;
		ret.x = vec.x;
		ret.y = vec.y;
		ret.z = vec.z;
		return ret;
	}

	AudioSystem::AudioSystem(Game* game)
		:m_Game(game),
		m_System(nullptr)
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
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create FMOD System: %s", FMOD_ErrorString(result));
			return false;
		}

		result = m_System->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialise FMOD System: %s", FMOD_ErrorString(result));
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
			SDL_Log("Bank '%s' is already loaded!", name.c_str());
			return;
		}

		FMOD_RESULT result;
		FMOD::Studio::Bank* bank = nullptr;
		result = m_System->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
		if (result != FMOD_OK)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load Bank '%s'", name.c_str());
			return;
		}

		const int maxPathLength = 512;

		m_Banks.emplace(name, bank);
		bank->loadSampleData();

		int numEvents;
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

		int numBuses;
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

	void AudioSystem::UnloadBank(const std::string& name)
	{
		auto iter = m_Banks.find(name);
		if (iter == m_Banks.end())
		{
			SDL_Log("Bank '%s' has not been loaded", name.c_str());
			return;
		}

		FMOD::Studio::Bank* bank = iter->second;
		const int maxPathLength = 512;

		int numEvents;
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

				auto eventi = m_Events.find(eventName);
				if (eventi != m_Events.end())
					m_Events.erase(eventi);
			}
		}

		int numBuses;
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
				
				auto busi = m_Buses.find(busName);
				if (busi != m_Buses.end())
					m_Buses.erase(busi);
			}
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

	SoundEvent AudioSystem::PlayEvent(const std::string& name)
	{
		unsigned int id = 0;
		auto iter = m_Events.find(name);
		if (iter != m_Events.end())
		{
			FMOD::Studio::EventInstance* eventInstance = nullptr;
			iter->second->createInstance(&eventInstance);
			if (eventInstance)
			{
				eventInstance->start();
				id = ++s_NextId;
				m_EventInstances.emplace(id, eventInstance);
			}
		}
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "'%s' event does not exist!", name.c_str());

		return SoundEvent(this, id);
	}

	void AudioSystem::Update(float deltaTime)
	{
		// Find any stopped events
		std::vector<unsigned int> completedEvents;
		for (auto& iter : m_EventInstances)
		{
			FMOD::Studio::EventInstance* event = iter.second;
			FMOD_STUDIO_PLAYBACK_STATE state;
			if (event->getPlaybackState(&state) == FMOD_STUDIO_PLAYBACK_STOPPED)
			{
				event->release();
				completedEvents.emplace_back(iter.first);
			}
		}

		// Remove the stopped events
		for (unsigned int id : completedEvents)
			m_EventInstances.erase(id);

		// update FMOD
		m_System->update();
	}

	void AudioSystem::SetListener(const glm::mat4& viewMatrix)
	{
		glm::mat4 invView = glm::inverse(viewMatrix);

		FMOD_3D_ATTRIBUTES listener;
		listener.position = VecToFMOD(glm::vec3(invView[3]));
		listener.forward = VecToFMOD(-glm::vec3(invView[2]));
		listener.up = VecToFMOD(glm::vec3(invView[1]));
		listener.velocity = VecToFMOD(glm::vec3(0));

		m_System->setListenerAttributes(0, &listener);
	}

	void AudioSystem::SetBusVolume(const std::string& name, const float value)
	{
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			iter->second->setVolume(value);
		}
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "'%s' bus doesn't exist/hasn't been loaded", name.c_str());
	}

	void AudioSystem::SetBusPaused(const std::string& name, const bool value)
	{
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			iter->second->setPaused(value);
		}
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "'%s' bus doesn't exist/hasn't been loaded", name.c_str());
	}

	float AudioSystem::GetBusVolume(const std::string& name) const
	{
		float value = 0.0f;
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			iter->second->getVolume(&value);
		}
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "'%s' bus doesn't exist/hasn't been loaded", name.c_str());
		return value;
	}

	float AudioSystem::GetBusPaused(const std::string& name) const
	{
		bool value = false;
		auto iter = m_Buses.find(name);
		if (iter != m_Buses.end())
		{
			iter->second->getPaused(&value);
		}
		else
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "'%s' bus doesn't exist/hasn't been loaded", name.c_str());
		return value;
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