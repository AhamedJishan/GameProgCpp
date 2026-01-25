#pragma once

#include "Component.h"

#include <vector>
#include "Engine/SoundEvent.h"

namespace jLab
{
	class AudioComponent : public Component
	{
	public:
		AudioComponent(class Actor* owner, int updateOrder = 200);
		~AudioComponent();

		void Update(float deltaTime) override;
		void OnUpdateWorldTransform() override;

		SoundEvent PlayEvent(const std::string& name);
		void StopAllEvents();

	protected:
		std::vector<SoundEvent> mEvents2D;
		std::vector<SoundEvent> mEvents3D;
	};
}