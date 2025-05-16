#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

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

		void ProcessInput(struct InputState& inputState);
		void UpdateActor(float deltaTime);

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

		class Game* GetGame() const { return m_Game; }
		State GetState() const { return m_State; }
		glm::mat4 GetWorldTransform() const { return m_WorldTransform; }
		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3 GetScale() const { return m_Scale; }
		glm::quat GetRotation() const { return m_Rotation; }
		glm::vec3 GetForward() const { return (m_Rotation * glm::vec3(0, 0, -1)); }
		glm::vec3 GetRight() const { return (m_Rotation * glm::vec3(1, 0, 0)); }
		glm::vec3 GetUp() const { return (m_Rotation * glm::vec3(0, 1, 0)); }

		void SetPosition(const glm::vec3 position) { m_Position = position; m_RecomputeTransform = true; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; m_RecomputeTransform = true; }
		void SetRotation(const glm::quat rotation) { m_Rotation = rotation; m_RecomputeTransform = true; }
		void Rotate(float angle, glm::vec3 axis)
		{
			glm::quat rot = glm::angleAxis(glm::radians(angle), axis);
			m_Rotation = glm::normalize(m_Rotation * rot);
			m_RecomputeTransform = true;
		}

	protected:
		virtual void Input(struct InputState& inputState);
		virtual void Update(float deltaTime);

		void ComputeTransform();

	protected:
		class Game* m_Game;
		State m_State;

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;

		bool m_RecomputeTransform;
		glm::mat4 m_WorldTransform;

		std::vector<class Component*> m_Components;
	};
}