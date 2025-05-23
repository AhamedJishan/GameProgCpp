#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class FPSActor : public Actor
	{
	public:
		FPSActor(class Game* game);

		void Input(struct InputState& inputState) override;
		void Update(float deltaTime) override;

		void SetVisible(bool visible);

	private:
		class MoveComponent* m_MoveComp;
		class Actor* m_FpsModel;
		class FPSCamera* m_Camera;
		class MeshRenderer* m_Mesh;

		const float m_Speed = 5.0f;
		glm::vec3 m_MoveDir;

		const float m_MaxAngularSpeed = 8.0f * 3.14f;
		float m_AngularSpeed;

		const float m_MaxPitchSpeed = 3.14f * 8.0f;
	};
}