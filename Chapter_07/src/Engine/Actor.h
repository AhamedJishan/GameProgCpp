#pragma once

#include <cstdint>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace jLab
{
	class Actor
	{
	public:
		enum State
		{
			E_Active,
			E_Paused,
			E_Dead
		};

		Actor(class Game* game);
		virtual ~Actor();

		void ProcessInput(const uint8_t* keyState);
		void Update(float deltaTime);

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);


		class Game* GetGame() { return m_Game; }
		State GetState() { return m_State; }
		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetScale() { return m_Scale; }
		glm::quat GetRotation() { return m_Rotation; }
		glm::vec3 GetForward() { return glm::normalize(m_Rotation * glm::vec3(0, 0, -1)); }
		glm::vec3 GetRight() { return glm::normalize(glm::cross(GetForward(), glm::vec3(0, 1, 0))); }
		glm::mat4 GetWorldTransform() { return m_WorldTransform; }

		void SetState(const State state) { m_State = state; }
		void SetPosition(const glm::vec3 position) { m_Position = position; m_RecomputeWorldTransform = true; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; m_RecomputeWorldTransform = true; }
		void SetRotation(const glm::quat rotation) { m_Rotation = rotation; m_RecomputeWorldTransform = true; }
		void Rotate(float angle, glm::vec3 axis) {
			m_Rotation = glm::normalize(m_Rotation * glm::angleAxis(angle, axis));
			m_RecomputeWorldTransform = true;
		}

	protected:
		virtual void InputActor(const uint8_t* keyState);
		virtual void UpdateActor(float deltaTime);

		void ComputeWorldTransform();

	protected:
		class Game* m_Game;
		State m_State;

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;

		bool m_RecomputeWorldTransform;
		glm::mat4 m_WorldTransform;

		std::vector<class Component*> m_Components;
	};
}