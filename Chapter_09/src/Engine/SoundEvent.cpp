#include "SoundEvent.h"

#include "AudioSystem.h"
#include <fmod_studio.hpp>

namespace jLab
{
	SoundEvent::SoundEvent()
		:m_System(nullptr),
		m_Id(0)
	{
	}

	SoundEvent::SoundEvent(AudioSystem* audioSystem, unsigned int id)
		:m_System(audioSystem),
		m_Id(id)
	{
	}
	
	bool SoundEvent::IsValid()
	{
		return (m_System && (m_System->GetEventInstance(m_Id) != nullptr));
	}
	
	void SoundEvent::Restart()
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->start();
	}
	
	void SoundEvent::Stop(bool allowFadeout)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			FMOD_STUDIO_STOP_MODE mode = allowFadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
			event->stop(mode);
		}
	}
	
	void SoundEvent::SetVolume(const float value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setVolume(value);
	}
	
	void SoundEvent::SetPitch(const float value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setPitch(value);
	}
	
	void SoundEvent::SetPaused(const bool value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setPaused(value);
	}
	
	void SoundEvent::SetParameter(const std::string& name, const float value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setParameterByName(name.c_str(), value);
	}
	
	float SoundEvent::GetVolume() const
	{
		float retValue = 0.0f;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			event->getVolume(&retValue);
		}
		return retValue;
	}
	
	float SoundEvent::GetPitch() const
	{
		float retValue = 0.0f;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			event->getPitch(&retValue);
		}
		return retValue;
	}
	
	float SoundEvent::GetPaused() const
	{
		bool retValue;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			event->getPaused(&retValue);
		}
		return retValue;
	}
	
	float SoundEvent::GetParameter(const std::string& name) const
	{
		float retValue = 0.0f;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			event->getParameterByName(name.c_str(), &retValue);
		}
		return retValue;
	}
	
	bool SoundEvent::Is3D()
	{
		bool retValue = false;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			FMOD::Studio::EventDescription* eventDesc= nullptr;
			event->getDescription(&eventDesc);
			eventDesc->is3D(&retValue);
		}
		return retValue;
	}
	
	void SoundEvent::Set3dAttributes(const glm::mat4& worldTrans)
	{
		glm::vec3 pos = glm::vec3(worldTrans[3]);
		glm::vec3 forward = -glm::vec3(worldTrans[2]);
		glm::vec3 up = glm::vec3(worldTrans[1]);

		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			FMOD_3D_ATTRIBUTES attrib;
			attrib.position = VecToFMOD(pos);
			attrib.forward = VecToFMOD(forward);
			attrib.up = VecToFMOD(up);
			attrib.velocity = { 0 ,0 ,0 };

			event->set3DAttributes(&attrib);
		}
	}
}