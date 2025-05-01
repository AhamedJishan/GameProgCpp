#pragma once

#include "Component.h"
#include <vector>
#include <string>

namespace jLab
{
	class AudioComponent : public Component
	{
	public:
		AudioComponent(class Actor* owner, int updateOrder = 200);
		~AudioComponent();

		void Update(float deltaTime) override;
		void OnUpdateWorldTransform() override;

		void PlayEvent(const std::string& name);
		void StopAllEvents();

	private:
		std::vector<class SoundEvent> m_Events2D;
		std::vector<class SoundEvent> m_Events3D;
	};
}