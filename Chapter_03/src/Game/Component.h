#pragma once

/**
* ---------------------------------------------------------------------------------------------------------
*												Component Class
* ---------------------------------------------------------------------------------------------------------
* The `Component` class serves as a base class for all modular components that an `Actor` might require.
* It provides the foundational functionality and structure for components in the game framework.
*
* - Constructor `Component(Actor* owner, int updateOrder = 100)`:
*   - Stores a reference to the owning `Actor`.
*   - Sets the `updateOrder`, which determines the component's update priority during the actor's update phase.
*     Components with lower `updateOrder` values are updated first.
*	- Calls `addComponent()` on the owning `Actor` to register itself with the actor.
*
* - Destructor:
*	 - Declared as `virtual` to ensure proper cleanup in derived classes when the owning actor is destroyed.
*	 - Automatically removes itself from the owning `Actor` by calling `removeComponent()` during destruction.
* 
* - Core Methods:
*   1. `update(float deltaTime)`:
*      - A virtual function intended to be overridden by derived classes.
*      - Used to define per-frame logic specific to the component.
*      - Called during the actor's `updateComponents` method as part of the game loop.
*
*   2. `getUpdateOrder()`:
*      - Returns the component's `updateOrder`, which indicates its priority for updates relative to other components.
*
* - Member Variables:
*   - `m_Owner`: A pointer to the owning `Actor`.
*   - `m_UpdateOrder`: An integer representing the update priority of the component (default: 100).
* ---------------------------------------------------------------------------------------------------------
*/


namespace ch3
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void update(float deltaTime);
		virtual void processInput(const uint8_t* keyState);

		int getUpdateOrder() const { return m_UppdateOrder; }

	protected:
		class Actor* m_Owner;
		int m_UppdateOrder;
	};
}