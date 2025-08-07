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

		void Update(float deltaTime);
		void ProcessInput(struct InputState& inputState);

		// TODO: UpdateComponent

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

		class Game* GetGame() const { return m_Game; }
		State GetState() const { return m_State; }
		glm::mat4 GetWorldTransform() const { return m_WorldTransform; }
		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3 GetScale() const { return m_Scale; }
		glm::quat GetRotation() const { return m_Rotation; }
		glm::vec3 GetForward() const { return m_Rotation * glm::vec3(0, 0, -1); }
		glm::vec3 GetRight() const { return m_Rotation * glm::vec3(1, 0, 0); }
		glm::vec3 GetUp() const { return m_Rotation * glm::vec3(0, 1, 0); }

		void SetState(const State state) { m_State = state; }
		void SetPosition(const glm::vec3 position) { m_Position = position; m_RecomputeWorldTransform = true; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; m_RecomputeWorldTransform = true; }
		void SetRotation(const glm::quat rotation) { m_Rotation = rotation; m_RecomputeWorldTransform = true; }
		void Rotate(float angle, const glm::vec3 axis)
		{
			glm::quat rot = glm::angleAxis(angle, glm::normalize(axis));
			m_Rotation = glm::normalize(m_Rotation * rot);
			m_RecomputeWorldTransform = true;
		}

		void LookAt(const glm::vec3& direction);

	protected:
		virtual void ActorUpdate(float deltaTime);
		virtual void ActorInput(struct InputState& inputState);

		void ComputeWorldTransform();

	protected:
		class Game* m_Game;
		State m_State;

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;

		glm::mat4 m_WorldTransform;
		bool m_RecomputeWorldTransform;

		std::vector<class Component*> m_Components;
	};
}