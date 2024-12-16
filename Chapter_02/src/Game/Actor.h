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

		Actor(Game* game);
		virtual ~Actor();

		void Update(float deltaTime);
		void UpdateComponent(float deltaTime);
		virtual void UpdateActor(float deltaTime);

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

	private:
		State m_State;

		Vector2 m_Position;
		float m_Rotation;


		std::vector<class Component*> m_Components;
		Game* m_Game;
	};
}