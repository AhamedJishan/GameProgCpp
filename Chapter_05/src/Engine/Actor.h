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

		void CalculateWorldTransform();

		Vector3 GetPosition() const { return m_Position; }
		void SetPosition(const Vector3 position) { m_Position = position; m_UpdateWorldTranform = true; }
		float GetRotation() const { return m_Rotation; }
		void SetRotation(const float rotation) { m_Rotation = rotation; m_UpdateWorldTranform = true; }
		Vector3 GetScale() const { return m_Scale; }
		void SetScale(const Vector3 scale) { m_Scale = scale; m_UpdateWorldTranform = true; }
		Matrix4 GetWorldTranform() { return m_WorldTransform; }

		Vector3 GetForward() const { return Vector3(Math::Cos(m_Rotation), Math::Sin(m_Rotation), 0); }

		State GetState() const { return m_State; }
		void SetState(const State state) { m_State = state; }

		Game* GetGame() { return m_Game; }

	protected:
		virtual void UpdateActor(float deltaTime);
		void UpdateComponent(float deltaTime);
		virtual void ActorInput(const uint8_t* keyState);

	private:
		class Game* m_Game;
		State m_State;

		Vector3 m_Position;
		float m_Rotation;
		Vector3 m_Scale;
		Matrix4 m_WorldTransform;
		bool m_UpdateWorldTranform;

		std::vector<class Component*> m_Components;
	};
}