#pragma once

#include "MoveComponent.h"

namespace jLab
{
	class InputComponent : public MoveComponent
	{
	public:
		InputComponent(class Actor* owner, int updateOrder = 100);

		void ProcessInput(const uint8_t* keyState) override;


		// GETTERS and SETTERS
		float GetMaxForwardSpeed() { return m_MaxForwardSpeed; }
		float GetMaxAngularSpeed() { return m_MaxAngularSpeed; }
		int GetForwardKey() { return m_ForwardKey; }
		int GetBackwardKey() { return m_BackwardKey; }
		int GetClockwiseKey() { return m_ClockwiseKey; }
		int GetCounterClockwiseKey() { return m_CounterClockwiseKey; }

		void SetMaxForwardSpeed(const float maxForwardSpeed) { m_MaxForwardSpeed = maxForwardSpeed; }
		void SetMaxAngularSpeed(const float maxAngularSpeed) { m_MaxAngularSpeed = maxAngularSpeed; }
		void SetForwardKey(const int forwardKey) { m_ForwardKey = forwardKey; }
		void SetBackwardKey(const int backwardKey) { m_BackwardKey = backwardKey; }
		void SetClockwiseKey(const int clockwiseKey) { m_ClockwiseKey = clockwiseKey; }
		void SetCounterClockwiseKey(const int counterClockwiseKey) { m_CounterClockwiseKey = counterClockwiseKey; }

	private:
		float m_MaxForwardSpeed;
		float m_MaxAngularSpeed;

		int m_ForwardKey;
		int m_BackwardKey;
		int m_ClockwiseKey;
		int m_CounterClockwiseKey;
	};
}