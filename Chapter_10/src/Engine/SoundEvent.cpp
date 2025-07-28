#include "SoundEvent.h"

#include "AudioSystem.h"
#include <fmod_studio.hpp>
#include <glm/vec3.hpp>

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
	SoundEvent::SoundEvent()
		:m_System(nullptr)
		,m_Id(0)
	{
	}

	SoundEvent::SoundEvent(AudioSystem* system, unsigned int id)
		:m_System(system)
		,m_Id(id)
	{
	}

	bool SoundEvent::IsValid()
	{
		return (m_System && m_System->GetEventInstance(m_Id) != nullptr);
	}
	
	void SoundEvent::Restart()
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->start();
	}
	
	void SoundEvent::Stop(bool allowFadeOut)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			FMOD_STUDIO_STOP_MODE mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
			event->stop(mode);
		}
	}
	
	void SoundEvent::SetPaused(bool value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setPaused(value);
	}
	
	void SoundEvent::SetVolume(float value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setVolume(value);
	}
	
	void SoundEvent::SetPitch(float value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setPaused(value);
	}
	
	void SoundEvent::SetParamater(const std::string& name, float value)
	{
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->setParameterByName(name.c_str(), value);
	}
	
	bool SoundEvent::GetPaused() const
	{
		bool retValue = false;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->getPaused(&retValue);

		return retValue;
	}
	
	float SoundEvent::GetVolume() const
	{
		float retValue = 0.0f;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->getVolume(&retValue);

		return retValue;
	}
	
	float SoundEvent::GetPitch() const
	{
		float retValue = 0.0f;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->getPitch(&retValue);

		return retValue;
	}
	
	float SoundEvent::GetParameter(const std::string& name) const
	{
		float retValue = 0.0f;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
			event->getParameterByName(name.c_str(), &retValue);

		return retValue;
	}

	bool SoundEvent::Is3D() const
	{
		bool retValue = false;
		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			FMOD::Studio::EventDescription* eventDescription = nullptr;
			event->getDescription(&eventDescription);
			eventDescription->is3D(&retValue);
		}
		return retValue;
	}

	void SoundEvent::Set3dAttribute(const glm::mat4& worldTransform)
	{
		glm::vec3 position = glm::vec3(worldTransform[3]);
		glm::vec3 forward = -glm::vec3(worldTransform[2]);
		glm::vec3 up = glm::vec3(worldTransform[1]);

		auto event = m_System ? m_System->GetEventInstance(m_Id) : nullptr;
		if (event)
		{
			FMOD_3D_ATTRIBUTES attrib;
			attrib.position = VecToFMOD(position);
			attrib.forward = VecToFMOD(forward);
			attrib.up = VecToFMOD(up);
			attrib.velocity = VecToFMOD(glm::vec3(0));
			event->set3DAttributes(&attrib);
		}
	}
}