#pragma once

#include <cstdint>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace jLab
{
	class Actor
	{
	public:
		enum State
		{
			EActive,
			EDead,
			EPaused
		};

		Actor(class Game* game);
		virtual ~Actor();

		void ProcessInput(const uint8_t* keyState);
		void Update(float deltaTime);

		// TODO: Add and removeComponent


		// GETTERS and SETTERS ----------------------------------------------------------
		class Game* GetGame() const { return m_Game; }
		glm::mat4 GetWorldTransform() const { return m_WorldTransform; }
		glm::vec3 GetPosition() const { return m_Position; }
		glm::quat GetRotation() const { return m_Rotation; }
		glm::vec3 GetScale() const { return m_Scale; }
		State GetState() const { return m_State; }
		glm::vec3 GetForward() const { return glm::normalize(glm::vec3(0, 0, 1) * m_Rotation); }

		void SetWorldTransform(const glm::mat4 transform) { m_WorldTransform = transform; }
		void SetPosition(const glm::vec3 position) { m_Position = position; m_RecomputeWorldTransform = true; }
		void SetRotation(const glm::quat rotation) { m_Rotation = rotation; m_RecomputeWorldTransform = true; }
		void SetScale(const glm::vec3 scale) { m_Scale = scale; m_RecomputeWorldTransform = true; }
		void SetState(const State state) { m_State = state; }

	protected:
		virtual void UpdateActor(float deltaTime);
		virtual void InputActor(const uint8_t* keyState);

		void CalculateWorldTransform();

	protected:
		class Game* m_Game;
		State m_State;

		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 m_WorldTransform;
		bool m_RecomputeWorldTransform;
	};
}