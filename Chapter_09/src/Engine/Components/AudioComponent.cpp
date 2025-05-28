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
	
	void AudioComponent::OnWorldTranformUpdate()
	{
		glm::mat4 world = m_Owner->GetWorldTransform();
		for (SoundEvent& event : m_Events3D)
			if (event.IsValid())
				event.Set3dAttributes(world);
	}
	
	SoundEvent AudioComponent::PlayEvent(const std::string& name)
	{
		SoundEvent soundEvent = m_Owner->GetGame()->GetAudioSystem()->PlayEvent(name);

		if (soundEvent.Is3D())
		{
			m_Events3D.emplace_back(soundEvent);
			soundEvent.Set3dAttributes(m_Owner->GetWorldTransform());
		}
		else
			m_Events2D.emplace_back(soundEvent);

		return soundEvent;
	}
	
	void AudioComponent::StopAllEvents()
	{
		for (SoundEvent& event : m_Events2D)
			event.Stop();
		for (SoundEvent& event : m_Events3D)
			event.Stop();

		m_Events2D.clear();
		m_Events3D.clear();
	}
}