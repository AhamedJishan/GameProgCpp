#pragma once

#include <string>
#include <glm/mat4x4.hpp>

namespace jLab
{
	class SoundEvent
	{
	public:
		SoundEvent();

		bool IsValid();
		bool Is3D();
		void Restart();
		void Stop(bool allowFadeOut = true);

		void Set3DAttributes(const glm::mat4& worldTranform);
		void SetPaused(bool pause);
		void SetVolume(float value);
		void SetPitch(float value);
		void SetParameter(const std::string& name, float value);

		bool GetPaused() const;
		float GetVolume() const;
		float GetPitch() const;
		float GetParameter(const std::string& name) const;

	protected:
		friend class AudioSystem;
		SoundEvent(class AudioSystem* system, unsigned int id);

	private:
		class AudioSystem* m_System;
		unsigned int m_Id;
	};
}