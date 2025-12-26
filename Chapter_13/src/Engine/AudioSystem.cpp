#include "AudioSystem.h"

#include <vector>
#include <glm/vec3.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

namespace jLab
{
	AudioSystem::AudioSystem(Game* game)
		:mGame(game)
		,mSystem(nullptr)
	{
	}

	bool AudioSystem::Init()
	{
		FMOD_RESULT result;

		result = FMOD::Studio::System::create(&mSystem); // Create the Studio System object.
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			return false;
		}

		// Initialize FMOD Studio, which will also initialize FMOD Core
		result = mSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			return false;
		}

		return true;
	}
	
	void AudioSystem::Shutdown()
	{
		UnloadAllBanks();

		if (mSystem)
			mSystem->release();
	}
	
	void AudioSystem::Update(float deltaTime)
	{
		std::vector<unsigned int> done;
		for (auto& iter : mEventInstances)
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

		for (unsigned int i : done)
			mEventInstances.erase(i);

		mSystem->update();
	}
	
	void AudioSystem::LoadBank(const std::string& name)
	{
		if (mBanks.find(name) == mBanks.end())
		{
			printf("[WARNING]: Tried to load bank '%s' which is already loaded!", name.c_str());
			return;
		}

		FMOD::Studio::Bank* bank = nullptr;
		FMOD_RESULT result = mSystem->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);

		const int maxPathLength = 512;
		if (result == FMOD_OK)
		{
			mBanks.emplace(name, bank);
			bank->loadSampleData();

			int numEvents;
			bank->getEventCount(&numEvents);
			if (numEvents > 0)
			{
				std::vector<FMOD::Studio::EventDescription*> events(numEvents);
				bank->getEventList(events.data(), numEvents, &numEvents);
				char eventName[maxPathLength];
				for (int i = 0; i < events.size(); i++)
				{
					FMOD::Studio::EventDescription* event = events[i];
					event->getPath(eventName, maxPathLength, 0);
					mEventDescriptions.emplace(eventName, event);
				}
			}

			int numBuses;
			bank->getBusCount(&numBuses);
			if (numBuses > 0)
			{
				std::vector<FMOD::Studio::Bus*> buses(numBuses);
				bank->getBusList(buses.data(), numBuses, &numBuses);
				char busName[maxPathLength];
				for (int i = 0; i < buses.size(); i++)
				{
					FMOD::Studio::Bus* bus = buses[i];
					bus->getPath(busName, maxPathLength, 0);
					mBuses.emplace(busName, bus);
				}
			}
		}
	}
	
	void AudioSystem::UnloadBank(const std::string& name)
	{
		auto iter = mBanks.find(name);
		if ( iter == mBanks.end())
		{
			printf("[WARNING]: Tried to unload bank '%s' which is not present!", name.c_str());
			return;
		}

		FMOD::Studio::Bank* bank = iter->second;

		const int maxPathLength = 512;

		int eventCount;
		bank->getEventCount(&eventCount);
		if (eventCount > 0)
		{
			std::vector<FMOD::Studio::EventDescription*> events(eventCount);
			bank->getEventList(events.data(), eventCount, &eventCount);
			char eventName[maxPathLength];
			for (int i = 0; i < events.size(); i++)
			{
				FMOD::Studio::EventDescription* event = events[i];
				event->getPath(eventName, eventCount, 0);

				auto eventIt = mEventDescriptions.find(eventName);
				if (eventIt != mEventDescriptions.end())
					mEventDescriptions.erase(eventIt);
			}
		}

		int numBuses;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[maxPathLength];
			for (int i = 0; i < buses.size(); i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				bus->getPath(busName, maxPathLength, 0);
				
				auto busIt = mBuses.find(busName);
				if (busIt != mBuses.end())
					mBuses.erase(busIt);
			}
		}

		bank->unloadSampleData();
		bank->unload();
		mBanks.erase(iter);
		
	}
	
	void AudioSystem::UnloadAllBanks()
	{
		for (auto& iter : mBanks)
		{
			iter.second->unloadSampleData();
			iter.second->unload();
		}

		mBanks.clear();
		mEventDescriptions.clear();
	}

	FMOD_VECTOR VecToFMOD(glm::vec3 vec)
	{
		FMOD_VECTOR vector;
		vector.x = vec.x;
		vector.y = vec.y;
		vector.z = vec.z;
		return vector;
	}
	
	void AudioSystem::SetListener(glm::mat4 view)
	{
		glm::mat4 inView = glm::inverse(view);

		FMOD_VECTOR up = VecToFMOD(inView[1]);
		FMOD_VECTOR forward = VecToFMOD(inView[2]);
		FMOD_VECTOR position = VecToFMOD(inView[3]);

		FMOD_3D_ATTRIBUTES listener;
		listener.up = up;
		listener.forward = forward;
		listener.position = position;
		listener.velocity = { 0, 0, 0 };

		mSystem->setListenerAttributes(0, &listener);
	}
	
	float AudioSystem::GetBusVolume(const std::string& name) const
	{
		auto iter = mBuses.find(name);
		if (iter == mBuses.end())
			return 0.0f;

		float volume;
		iter->second->getVolume(&volume);
		return volume;
	}
	
	bool AudioSystem::GetBusPaused(const std::string& name) const
	{
		auto iter = mBuses.find(name);
		if (iter == mBuses.end())
			return false;

		bool paused;
		iter->second->getPaused(&paused);
		return paused;
	}
	
	void AudioSystem::SetBusVolume(const std::string& name, float volume)
	{
		auto iter = mBuses.find(name);
		if (iter == mBuses.end())
			return;

		iter->second->setVolume(volume);
	}
	
	void AudioSystem::SetBusPaused(const std::string& name, bool pause)
	{
		auto iter = mBuses.find(name);
		if (iter == mBuses.end())
			return;

		iter->second->setPaused(pause);
	}
}