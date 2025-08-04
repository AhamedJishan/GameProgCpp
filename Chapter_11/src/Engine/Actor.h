#pragma once

#include "InputSystem.h"
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

		State GetState() const { return m_State; }
		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3 GetScale() const { return m_Scale; }
		glm::quat GetRotation() const { return m_Rotation; }
		glm::mat4 GetWorldTransform() const { return m_WorldTransform; }

		void ComputeWorldTransform();

		void SetState(State state) { m_State = state; }
		void SetPosition(const glm::vec3& position) { m_Position = position; m_RecomputeWorldTransform = true; }
		void SetScale(const glm::vec3& scale) { m_Scale = scale; m_RecomputeWorldTransform = true; }
		void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; m_RecomputeWorldTransform = true; }
		void Rotate(float angle, glm::vec3 axis)
		{
			m_Rotation = m_Rotation * glm::angleAxis(angle, axis);
		}

	private:
		friend class Game;
		void UpdateActor(float deltaTime);
		void InputActor(const InputState& inputState);

	protected:
		virtual void Update(float deltaTime);
		virtual void Input(const InputState& inputState);

	protected:
		class Game* m_Game;
		State m_State;

		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;

		glm::mat4 m_WorldTransform;
		bool m_RecomputeWorldTransform;
	};
}