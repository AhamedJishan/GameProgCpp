#pragma once

#include <string>
#include <glm/mat4x4.hpp>

namespace jLab
{
	class SoundEvent
	{
	public:
		bool IsValid() const;
		void Restart();
		void Stop(bool allowFadeout = false);

		void SetPaused(bool value);
		void SetVolume(float value);
		void SetPitch(float value);
		void SetParamater(const std::string& name, float value);

		bool GetPaused() const;
		float GetVolume() const;
		float GetPitch() const;
		float GetParamater(const std::string& name) const;

		bool Is3D() const;
		void Set3DAttributes(glm::mat4 worldTransform);

	protected:
		friend class AudioSystem;
		SoundEvent(class AudioSystem* system, unsigned int id);

	private:
		class AudioSystem* mSystem;
		unsigned int mId;
	};
}