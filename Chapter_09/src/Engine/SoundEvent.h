#pragma once

#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace jLab
{
	class SoundEvent
	{
	public:
		SoundEvent();

		bool IsValid();
		void Restart();
		void Stop(bool allowFadeout = true);

		void SetVolume(const float value);
		void SetPitch(const float value);
		void SetPaused(const bool value);
		void SetParameter(const std::string& name, const float value);

		float GetVolume() const;
		float GetPitch() const;
		float GetPaused() const;
		float GetParameter(const std::string& name) const;

		bool Is3D();
		void Set3dAttributes(const glm::mat4& worldTrans);

	protected:
		friend class AudioSystem;
		SoundEvent(class AudioSystem* audioSystem, unsigned int id);

	private:
		class AudioSystem* m_System;
		unsigned int m_Id;
	};
}