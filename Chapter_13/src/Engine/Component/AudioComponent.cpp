#include "AudioComponent.h"

#include <glm/mat4x4.hpp>
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
		// Delete all 2D Events
		auto iter = mEvents2D.begin();
		while (iter != mEvents2D.end())
		{
			if (!iter->IsValid())
				iter = mEvents2D.erase(iter);
			else
				iter++;
		}

		// Delete all 3d Events
		iter = mEvents3D.begin();
		while (iter != mEvents3D.end())
		{
			if (!iter->IsValid())
				iter = mEvents3D.erase(iter);
			else
				iter++;
		}
	}

	void AudioComponent::OnUpdateWorldTransform()
	{
		glm::mat4 worldTransform = mOwner->GetWorldTransformMatrix();

		for (auto& event : mEvents3D)
			if (event.IsValid())
				event.Set3DAttributes(worldTransform);
	}

	SoundEvent AudioComponent::PlayEvent(const std::string& name)
	{
		SoundEvent event = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);

		if (event.Is3D())
		{
			mEvents3D.emplace_back(event);
			event.Set3DAttributes(mOwner->GetWorldTransformMatrix());
		}
		else
			mEvents2D.emplace_back(event);

		return event;
	}

	void AudioComponent::StopAllEvents()
	{
		for (auto& event : mEvents2D)
			event.Stop();

		for (auto& event : mEvents3D)
			event.Stop();

		mEvents2D.clear();
		mEvents3D.clear();
	}
}