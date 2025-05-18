#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class FollowActor : public Actor
	{
	public:
		FollowActor(class Game* game);

		void Input(struct InputState& inputState) override;
		void Update(float deltaTime) override;

		class FollowCamera* GetFollowCamera() const { return m_Camera; }

	private:
		class FollowCamera* m_Camera;
		class MoveComponent* m_MoveComp;

		const float m_Speed = 10.0f;
		glm::vec3 m_MoveDir;

		const float m_MaxAngularSpeed = 2.75f;
		float m_AngularSpeed;
	};
}