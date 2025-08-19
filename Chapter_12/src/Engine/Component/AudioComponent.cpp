#include "AudioComponent.h"

#include "Engine/Actor.h"
#include "Engine/Game.h"
#include "Engine/AudioSystem.h"

namespace jLab
{
	AudioComponent::AudioComponent(Actor* owner, int updateOrder)
		:Component(owner, updateOrder)
	{
	}
	
	AudioComponent::~AudioComponent()
	{
		StopAllEvents();
	}
	
	void AudioComponent::Update(float deltaTime)
	{
		// Remove all Invalid events
		auto iter = m_Events2D.begin();
		while (iter != m_Events2D.end())
		{
			if (!iter->IsValid())
				iter = m_Events2D.erase(iter);
			else
				iter++;
		}

		iter = m_Events3D.begin();
		while (iter != m_Events3D.end())
		{
			if (!iter->IsValid())
				iter = m_Events3D.erase(iter);
			else
				iter++;
		}
	}
	
	void AudioComponent::OnUpdateWorldTransform()
	{
		for (SoundEvent event : m_Events3D)
			if(event.IsValid())
				event.Set3dAttribute(m_Owner->GetWorldTransform());
	}
	
	SoundEvent AudioComponent::PlayEvent(const std::string& name)
	{
		SoundEvent event = m_Owner->GetGame()->GetAudioSystem()->PlayEvent(name);

		if (event.Is3D())
		{
			m_Events3D.emplace_back(event);
			event.Set3dAttribute(m_Owner->GetWorldTransform());
		}
		else
			m_Events2D.emplace_back(event);

		return event;
	}
	
	void AudioComponent::StopAllEvents()
	{
		for (SoundEvent event : m_Events2D)
			event.Stop();

		for (SoundEvent event : m_Events3D)
			event.Stop();

		m_Events2D.clear();
		m_Events3D.clear();
	}
}