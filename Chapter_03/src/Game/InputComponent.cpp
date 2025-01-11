#include "InputComponent.h"

#include "SDL/SDL.h"

namespace ch3
{
	InputComponent::InputComponent(Actor* owner)
		:MoveComponent(owner)
	{
	}

	void InputComponent::processInput(const uint8_t* keyState)
	{
		float forwardSpeed = 0.0f;
		if (keyState[m_ForwardKey])
			forwardSpeed += m_MaxForwardSpeed;
		if (keyState[m_BackKey])
			forwardSpeed -= m_MaxForwardSpeed;

		float angularSpeed = 0.0f;
		if (keyState[m_ClockWiseKey])
			angularSpeed += m_MaxAngularSpeed;
		if (keyState[m_AntiClockWiseKey])
			angularSpeed -= m_MaxAngularSpeed;

		MoveComponent::setForwardSpeed(forwardSpeed);
		MoveComponent::setAngularSpeed(angularSpeed);
	}
}