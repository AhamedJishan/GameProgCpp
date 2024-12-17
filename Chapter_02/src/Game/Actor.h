#pragma once

#include <vector>

#include "Math.h"
#include "Game.h"

namespace ch2
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
		virtual void UpdateActor(float deltaTime);

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

		State GetState() { return m_State; }
		Vector2 GetPosition() { return m_Position; }
		float GetRotation() { return m_Rotation; }
		float GetScale() { return m_Scale; }
		Game* GetGame() { return m_Game; }

	private:
		State m_State;

		Vector2 m_Position;
		float m_Rotation;
		float m_Scale;

		std::vector<class Component*> m_Components;
		class Game* m_Game;
	};
}