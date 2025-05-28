#pragma once

#include "Component.h"
#include "Engine/SoundEvent.h"
#include <string>
#include <vector>

namespace jLab
{
	class AudioComponent : public Component
	{
	public:
		AudioComponent(class Actor* owner, int updateOrder = 200);
		~AudioComponent();

		void Update(float deltaTime) override;
		void OnWorldTranformUpdate() override;

		SoundEvent PlayEvent(const std::string& name);
		void StopAllEvents();
	private:
		std::vector<SoundEvent> m_Events2D;
		std::vector<SoundEvent> m_Events3D;
	};
}