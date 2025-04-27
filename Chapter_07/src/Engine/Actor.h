#pragma once

#include <cstdint>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include<glm/gtc/quaternion.hpp>

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

		// TODO: Component Management

		class Game* GetGame() { return m_Game; }
		State GetState() { return m_State; }
		void SetState(const State state) { m_State = state; }

		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetScale() { return m_Scale; }
		glm::quat GetRotation() { return m_Rotation; }

		void SetPosition(const glm::vec3 position) { m_Position = position; m_RecomputeWorldTransform = true; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; m_RecomputeWorldTransform = true; }
		void SetRotation(const glm::quat rotation) { m_Rotation = rotation; m_RecomputeWorldTransform = true; }
		void Rotate(float angle, glm::vec3 axis) {
			m_Rotation = glm::normalize(glm::angleAxis(glm::radians(angle), axis) * m_Rotation);
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
	};
}