#pragma once

#include "MoveComponent.h"
#include <stdint.h>

namespace ch3
{
	class InputComponent : public MoveComponent
	{
	public:
		InputComponent(class Actor* owner);

		void processInput(const uint8_t* keyState) override;

		// Getters and Setter
		float getMaxForwardSpeed() const { return m_MaxForwardSpeed; }
		void setMaxForwardSpeed(float speed) { m_MaxForwardSpeed = speed; }

		float getMaxAngularSpeed() const { return m_MaxAngularSpeed; }
		void setMaxAngularSpeed(float speed) { m_MaxAngularSpeed = speed; }

		int getForwardKey() const { return m_ForwardKey; }
		void setForwardKey(int key) { m_ForwardKey = key; }

		int getBackKey() const { return m_BackKey; }
		void setBackKey(int key) { m_BackKey = key; }

		int getClockWiseKey() const { return m_ClockWiseKey; }
		void setClockWiseKey(int key) { m_ClockWiseKey = key; }

		int getAntiClockWiseKey() const { return m_AntiClockWiseKey; }
		void setAntiClockWiseKey(int key) { m_AntiClockWiseKey = key; }

	private:
		float m_MaxForwardSpeed;
		float m_MaxAngularSpeed;

		int m_ForwardKey;
		int m_BackKey;
		int m_ClockWiseKey;
		int m_AntiClockWiseKey;
	};
}

