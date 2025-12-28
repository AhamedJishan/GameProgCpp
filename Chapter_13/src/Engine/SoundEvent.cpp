#include "SoundEvent.h"

#include "AudioSystem.h"
#include <fmod_studio.hpp>

namespace jLab
{
	SoundEvent::SoundEvent(AudioSystem* system, unsigned int id)
		:mSystem(system)
		,mId(id)
	{
	}

	bool SoundEvent::IsValid() const
	{
		return (mSystem && mSystem->GetEventInstance(mId) != nullptr);
	}
	
	void SoundEvent::Restart()
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->start();
	}
	
	void SoundEvent::Stop(bool allowFadeout)
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
		{
			FMOD_STUDIO_STOP_MODE mode = allowFadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
			event->stop(mode);
		}
	}
	
	void SoundEvent::SetPaused(bool value)
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->setPaused(value);
	}
	
	void SoundEvent::SetVolume(float value)
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->setVolume(value);
	}
	
	void SoundEvent::SetPitch(float value)
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->setPitch(value);
	}
	
	void SoundEvent::SetParamater(const std::string& name, float value)
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->setParameterByName(name.c_str(), value);
	}
	
	bool SoundEvent::GetPaused() const
	{
		bool value = false;
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->getPaused(&value);
		return value;
	}
	
	float SoundEvent::GetVolume() const
	{
		float value = 0.0f;
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->getVolume(&value);
		return value;
	}
	
	float SoundEvent::GetPitch() const
	{
		float value = 0.0f;
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->getPitch(&value);
		return value;
	}
	
	float SoundEvent::GetParamater(const std::string& name) const
	{
		float value = 0.0f;
		auto event = mSystem->GetEventInstance(mId);
		if (event)
			event->getParameterByName(name.c_str(), &value);
		return value;
	}
	
	bool SoundEvent::Is3D() const
	{
		bool is3D = false;
		auto event = mSystem->GetEventInstance(mId);
		if (event)
		{
			FMOD::Studio::EventDescription* eventDescription = nullptr;
			event->getDescription(&eventDescription);
			eventDescription->is3D(&is3D);
		}
		return is3D;
	}

	FMOD_VECTOR VecToFMODVec(glm::vec3 vec)
	{
		FMOD_VECTOR vector;
		vector.x = vec.x;
		vector.y = vec.y;
		vector.z = vec.z;
		return vector;
	}
	
	void SoundEvent::Set3DAttributes(glm::mat4 worldTransform)
	{
		auto event = mSystem->GetEventInstance(mId);
		if (event)
		{
			FMOD_3D_ATTRIBUTES attributes;

			attributes.up = VecToFMODVec(worldTransform[1]);
			attributes.forward = VecToFMODVec(worldTransform[2]);
			attributes.position = VecToFMODVec(worldTransform[3]);
			attributes.velocity = { 0, 0, 0 };

			event->set3DAttributes(&attributes);
		}
	}
}