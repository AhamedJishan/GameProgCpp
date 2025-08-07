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
		void Restart();
		void Stop(bool allowFadeOut = true);

		void SetPaused(bool value);
		void SetVolume(float value);
		void SetPitch(float value);
		void SetParamater(const std::string& name, float value);

		bool GetPaused() const;
		float GetVolume() const;
		float GetPitch() const;
		float GetParameter(const std::string& name) const;

		bool Is3D() const;
		void Set3dAttribute(const glm::mat4& worldTransform);

	protected:
		friend class AudioSystem;
		SoundEvent(class AudioSystem* system, unsigned int id);

	private:
		unsigned int m_Id;
		class AudioSystem* m_System;
	};
}