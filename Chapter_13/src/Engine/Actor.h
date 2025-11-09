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
		enum class State
		{
			Active,
			Paused,
			Dead
		};

		Actor(class Game* game);
		virtual ~Actor();

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);

		class Game* GetGame() const { return mGame; }
		State GetState() const { return mState; }

		glm::vec3 GetScale() const { return mScale; }
		glm::vec3 GetPosition() const { return mPosition; }
		glm::quat GetRotation() const { return mRotation; }

		glm::vec3 GetUp() const { return mRotation * glm::vec3(0, 1, 0); }
		glm::vec3 GetRight() const { return mRotation * glm::vec3(1, 0, 0); }
		glm::vec3 GetForward() const { return mRotation * glm::vec3(0, 0, -1); }

		glm::mat4 GetWorldTransformMatrix() const { return mWorldTransform; }

		void SetState(State state) { mState = state; }
		void SetScale(const glm::vec3 scale) { mScale = scale; mRecomputeWorldTransform = true; }
		void SetPosition(const glm::vec3 position) { mPosition = position; mRecomputeWorldTransform = true; }
		void SetRotation(const glm::quat rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

		void Rotate(float angle, glm::vec3 axis)
		{
			glm::quat rot = glm::angleAxis(angle, glm::normalize(axis));
			mRotation = glm::normalize(mRotation * rot);
			mRecomputeWorldTransform = true;
		}

		void LookAt(const glm::vec3& direction);

	protected:
		friend class Game;
		void ProcessUpdate(float deltaTime);
		void ProcessInput(const struct InputState inputState);

		void ComputeWorldTransform();

		// To be implemented by child classes
		virtual void Update(float deltaTime);
		virtual void Input(struct InputState inputState);

	protected:
		State mState;
		class Game* mGame;

		glm::vec3 mScale;
		glm::vec3 mPosition;
		glm::quat mRotation;

		glm::mat4 mWorldTransform;
		bool mRecomputeWorldTransform;

		std::vector<class Component*> mComponents;
	};
}