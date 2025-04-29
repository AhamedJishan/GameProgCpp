#include "SoundEvent.h"

#include "AudioSystem.h"

namespace jLab
{
	SoundEvent::SoundEvent(AudioSystem* system, unsigned int id)
		:m_System(system),
		m_Id(id)
	{
	}

	SoundEvent::SoundEvent()
		:m_System(nullptr),
		m_Id(0)
	{
	}
	
	bool SoundEvent::IsValid()
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		if (event)
			return event->isValid();

		return false;
	}
	
	void SoundEvent::Restart()
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		if (event)
			event->start();
	}
	
	void SoundEvent::Stop(bool allowFadeOut)
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;

		if (event)
			event->stop(mode);
	}
	
	void SoundEvent::SetPaused(bool pause)
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		if (event)
			event->setPaused(pause);
	}
	
	void SoundEvent::SetVolume(float value)
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		if (event)
			event->setVolume(value);
	}
	
	void SoundEvent::SetPitch(float value)
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		if (event)
			event->setPitch(value);
	}
	
	void SoundEvent::SetParameter(const std::string& name, float value)
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		if (event)
			event->setParameterByName(name.c_str(), value);
	}
	
	bool SoundEvent::GetPaused() const
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		
		bool paused = false;
		if (event)
			event->getPaused(&paused);

		return paused;
	}
	
	float SoundEvent::GetVolume() const
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		float volume = 0.0f;
		if (event)
			event->getVolume(&volume);

		return volume;
	}
	
	float SoundEvent::GetPitch() const
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		float pitch = 0.0f;
		if (event)
			event->getPitch(&pitch);

		return pitch;
	}
	
	float SoundEvent::GetParameter(const std::string& name) const
	{
		FMOD::Studio::EventInstance* event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;

		float value = 0.0f;
		if (event)
			event->getParameterByName(name.c_str(), &value);

		return value;
	}
}