#pragma once

/**
* ---------------------------------------------------------------------------------------------------------
*                                             MoveComponent Class
* ---------------------------------------------------------------------------------------------------------
* Provides movement functionality to the owning Actor.
* This component modifies the rotation and position of the owning Actor based on specified angular
* and forward speeds during the update loop.
* 
* - Constructor `MoveComponent(class Actor* owner, int updateOrder = 10)`:
*   - Initializes the `MoveComponent` by calling the parent `Component` constructor with the owner
*     and update order.
* 
* - Core Methods:
*	1. update(float deltaTime):
*		- Overrides the parent class's update method.
*		- When `m_AngularSpeed` and `m_ForwardSpeed` is not near zero, it modifies the position and rotation
*		  of the owner Actor based on the angular speed, forward speed and deltaTime.
* 
* - Getters and Setters:
*	- Access and modify the following member variables:
*		- `m_AngularSpeed`: Controls rotation (radians/second)
*		- `m_ForwardSpeed`: Controls forward movement (units/seconds)
* ---------------------------------------------------------------------------------------------------------
*/

#include "Component.h"

namespace ch3
{
	class MoveComponent : public Component
	{
	public:

		MoveComponent(class Actor* owner, int updateOrder = 10);

		void update(float deltaTime) override;

		float getAngularSpeed() const { return m_AngularSpeed; }
		void setAngularSpeed(float angularSpeed) { m_AngularSpeed = angularSpeed; }

		float getForwardSpeed() const { return m_ForwardSpeed; }
		void setForwardSpeed(float forwardSpeed) { m_ForwardSpeed = forwardSpeed; }

	private:
		// Controls rotation (radians/second)
		float m_AngularSpeed;
		// Controls forward movement (units/seconds)
		float m_ForwardSpeed;
	};
}