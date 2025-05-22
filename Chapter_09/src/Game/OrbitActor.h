#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class OrbitActor : public Actor
	{
	public:
		OrbitActor(class Game* game);


		void Input(struct InputState& inputState) override;
		void Update(float deltaTime) override;

		void SetVisible(bool visible);

	private:
		class OrbitCamera* m_Camera;
		class MeshRenderer* m_Mesh;

		const float m_MaxPitchInput = 1.0f;
		const float m_MaxYawInput = 1.0f;

		float m_PitchInput ;
		float m_YawInput ;
	};
}