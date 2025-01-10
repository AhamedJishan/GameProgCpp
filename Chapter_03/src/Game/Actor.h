#pragma once

/**
* ---------------------------------------------------------------------------------------------------------
*                                             Actor Class
* ---------------------------------------------------------------------------------------------------------
* Represents a game object in the project. Every game object will inherit from the Actor class.
* The class encapsulates the state, position, scale, rotation, and a reference to the game object,
* while also providing functionality for updating the object and its components during the game loop.
*
* - Constructor `Actor(Game* game)`:
*   - Initializes the following member variables:
*     - `m_State`: Represents the current state of the actor (possible states: `EActive`, `EPaused`, `EDead`).
*     - `m_Position`: A `Vector2` object storing the actor's position (initialized to `(0, 0)`).
*     - `m_Rotation`: A `float` storing the actor's rotation (initialized to `0.0` radians).
*     - `m_Scale`: A `float` storing the actor's scale (initialized to `0.0`).
*     - `m_Game`: A pointer to the `Game` object that manages this actor.
*   - Automatically adds the actor to the game's list of actors.
*
* - Destructor `~Actor()`:
*   - Declared `virtual` to ensure proper cleanup of resources in derived classes when the actor is destroyed.
*   - Removes the actor from the game's list of actors.
*   - Deletes every component stored in `m_Components`.
*
* - Core Methods:
*   1. `update(float deltaTime)`:
*      - A public method called by the `Game` class during each game loop iteration.
*      - Updates the actor by:
*        - Calling `updateComponents(float deltaTime)` to update all attached components.
*        - Calling `updateActor(float deltaTime)` to update the actor's logic. Designed to be overridden in derived classes.
*
*   2. `updateComponents(float deltaTime)`:
*      - A private method responsible for updating all components attached to this actor in the order of their update priorities.
*      - Loops through `m_Components` and calls `update(deltaTime)` on each component.
*
*   3. `updateActor(float deltaTime)`:
*      - A `protected` virtual method intended to be overridden by derived classes.
*      - Allows child classes to define custom behavior executed during each game update.
*
* - Component Management:
*   - `addComponent(Component* component)`:
*     - Adds a new component to the actor, maintaining an order based on the component's update priority (`getUpdateOrder()`).
*   - `removeComponent(Component* component)`:
*     - Removes a specified component from the actor, if it exists.
*
* - Getters and Setters:
*   Provides access to and modification of the following member variables:
*     - `m_State`: Get or set the actor's current state.
*     - `m_Position`: Get or set the actor's position as a `Vector2`.
*     - `m_Rotation`: Get or set the actor's rotation (in radians).
*     - `m_Scale`: Get or set the actor's scale.
*     - `m_Game`: Get the reference to the `Game` object that owns this actor.
*     - `getForward()`: Computes and returns the forward-facing direction as a `Vector2` based on the actor's rotation.
*
* ---------------------------------------------------------------------------------------------------------
*/



#include <vector>
#include "Math.h"

namespace ch3
{
	class Actor
	{
	public:
		enum State
		{
			EActive,
			EPaused,
			EDead
		};

		Actor(class Game* game);
		virtual ~Actor();

		void update(float deltaTime);

		void addComponent(class Component* component);
		void removeComponent(class Component* component);


		// Getters and Setters

		State	getState() const				{ return m_State; }
		void	setState(State state)			{ m_State = state; }

		Vector2 getPosition() const				{ return m_Position; }
		void	setPosition(Vector2 position)	{ m_Position = position; }

		float	getRotation() const				{ return m_Rotation; }
		void	setRotation(float rotation)		{ m_Rotation = rotation; }

		float	getScale() const				{ return m_Scale; }
		void	setScale(float position)		{ m_Scale = m_Scale; }

		Vector2 getForward() const				{ return Vector2( Math::Cos(m_Rotation), -Math::Sin(m_Rotation) ); }

		Game*	getGame() const					{ return m_Game; }


	protected:
		virtual void updateActor(float deltaTime);

	private:
		void updateComponents(float deltaTime);

	private:
		State m_State;

		Vector2 m_Position;
		float m_Rotation;
		float m_Scale;

		class Game* m_Game;

		std::vector<class Component*> m_Components;
	};
}