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

	private:
		class FollowCamera* m_Camera;
		class MoveComponent* m_MoveComp;

		const float m_Speed = 5.0f;
		glm::vec3 m_MoveDir;

		const float m_MaxAngularSpeed = 3.14f / 2.0f;
		float m_AngularSpeed;
	};
}