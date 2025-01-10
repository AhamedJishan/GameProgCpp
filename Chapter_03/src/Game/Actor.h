#pragma once

/**
 * ---------------------------------------------------------------------------------------------------------
 *												Actor Class
 * ---------------------------------------------------------------------------------------------------------
* Represents a game object in the project. Every game object will inherit from the Actor class.
* The class encapsulates the state, position, scale, rotation, and game reference for an object and provides
* functionality for updating the object and its components during the game loop.
*
* - Constructor `Actor(Game* game)`:
*   - Initializes the following member variables:
*     - `m_State`: Represents the current state of the actor (possible states: `EActive`, `EPaused`, `EDead`).
*     - `m_Position`: A `Vector2` object storing the actor's position (initialized to `(0, 0)`).
*     - `m_Scale`: A `float` storing the actor's scale (initialized to `0.0`).
*     - `m_Rotation`: A `float` storing the actor's rotation (initialized to `0.0`).
*     - `m_Game`: A reference to the `Game` object that manages this actor.
*
* - Destructor:
*   - Declared `virtual` to ensure proper cleanup of resources in derived classes when the actor is destroyed.
*	- deletes every component stored in the `m_Components` one by one.
*
* - Core Methods:
*   1. `update(float deltaTime)`:
*      - A public method called by the `Game` class during each game loop iteration.
*      - Calls:
*        - `updateComponents(float deltaTime)`: Updates all attached components (currently a placeholder).
*        - `updateActor(float deltaTime)`: Updates the actor's logic. Designed to be overridden in derived classes.
*   2. `updateComponents(float deltaTime)`:
*      - A private method responsible for updating all components attached to this actor.
*      - Currently a placeholder for future functionality.
*   3. `updateActor(float deltaTime)`:
*      - A `protected` virtual method intended to be overridden by derived classes.
*      - Allows child classes to define custom behavior executed during each game update.
*
* - Getters and Setters:
*   Provides access to and modification of the following member variables:
*     - `m_State`: Get or set the actor's current state.
*     - `m_Position`: Get or set the actor's position.
*     - `m_Rotation`: Get or set the actor's rotation.
*     - `m_Scale`: Get or set the actor's scale.
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