#pragma once

#include <string>

namespace jLab
{
	class SoundEvent
	{
	public:
		SoundEvent();

		bool IsValid();
		void Restart();
		void Stop(bool allowFadeOut = true);

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