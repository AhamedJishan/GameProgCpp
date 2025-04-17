#include "InputComponent.h"

#include "Actor.h"
#include <SDL/SDL.h>

namespace jLab
{
	InputComponent::InputComponent(Actor* owner, int updateOrder)
		:MoveComponent(owner, updateOrder),
		m_MaxForwardSpeed(0),
		m_MaxAngularSpeed(0),
		m_ForwardKey(SDL_SCANCODE_UP),
		m_BackwardKey(SDL_SCANCODE_DOWN),
		m_ClockwiseKey(SDL_SCANCODE_RIGHT),
		m_CounterClockwiseKey(SDL_SCANCODE_LEFT)
	{
	}
	
	void InputComponent::ProcessInput(const uint8_t* keyState)
	{
		float forwardSpeed = 0;
		if (keyState[m_ForwardKey])
			forwardSpeed += m_MaxForwardSpeed;
		if (keyState[m_BackwardKey])
			forwardSpeed -= m_MaxForwardSpeed;

		float angularSpeed = 0;
		if (keyState[m_ClockwiseKey])
			angularSpeed -= m_MaxAngularSpeed;
		if (keyState[m_CounterClockwiseKey])
			angularSpeed += m_MaxAngularSpeed;

		MoveComponent::SetForwardSpeed(forwardSpeed);
		MoveComponent::SetAngularSpeed(angularSpeed);
	}
}