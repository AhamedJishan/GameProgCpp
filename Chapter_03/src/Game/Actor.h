#pragma once

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


		// Getters and Setters

		State	getState() const				{ return m_State; }
		void	setState(State state)			{ m_State = state; }

		Vector2 getPosition() const				{ return m_Position; }
		void	setPosition(Vector2 position)	{ m_Position = position; }

		float	getRotation() const				{ return m_Rotation; }
		void	setRotation(float rotation)		{ m_Rotation = rotation; }

		float	getScale() const				{ return m_Scale; }
		void	setScale(float position)		{ m_Scale = m_Scale; }


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
	};
}