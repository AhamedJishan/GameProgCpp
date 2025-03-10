#pragma once

#include <cstdint>
#include "util/Math.h"

namespace jLab
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

		void Update(float deltaTime);
		void UpdateComponent(float deltaTime);
		// To be implemented by the child Actor
		virtual void UpdateActor(float deltaTime);

		void ProcessInput(const uint8_t* keyboardState);
		virtual void ActorInput(const uint8_t* keyboardState);


		void SetPosition (Vector2 position)	{ m_Position = position; }
		Vector2& GetPosition () { return m_Position; }
		Vector2 GetForward () { return Vector2(Math::Cos(m_Rotation), -Math::Sin(m_Rotation)); }

		void SetRotation (float rotation) { m_Rotation = rotation; }
		float GetRotation () { return m_Rotation; }

		void SetScale (float scale) { m_Scale = scale; }
		float GetScale () { return m_Scale; }

		void SetState(State state) { m_State = state; }
		State GetState() { return m_State; }

		Game* GetGame() { return m_Game; }

	private:
		Vector2 m_Position;
		float m_Rotation;
		float m_Scale;

		State m_State;
		class Game* m_Game;
	};
}