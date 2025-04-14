#pragma once

#include "util/Math.h"
#include <cstdint>
#include <vector>

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
		~Actor();

		void Update(float deltaTime);
		void ProcessInput(const uint8_t* keyState);

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

		Vector2 GetPosition() const { return m_Position; }
		void SetPosition(const Vector2 position) { m_Position = position; }
		float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; }
		float GetScale() const { return m_Scale; }
		void SetScale(const float scale) { m_Scale = scale; }

		Vector2 GetForward() const { return Vector2(Math::Cos(m_Rotation), Math::Sin(m_Rotation)); }

		State GetState() const { return m_State; }
		void SetState(const State state) { m_State = state; }

		Game* GetGame() { return m_Game; }

	protected:
		virtual void UpdateActor(float deltaTime);
		void UpdateComponent(float deltaTime);
		virtual void ActorInput(const uint8_t* keyState);

	private:
		class Game* m_Game;

		Vector2 m_Position;
		float m_Rotation;
		float m_Scale;

		State m_State;

		std::vector<class Component*> m_Components;
	};
}