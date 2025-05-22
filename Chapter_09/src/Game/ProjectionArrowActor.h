#pragma once

#include "Engine/Actor.h"

namespace jLab
{
	class ProjectionArrowActor : public Actor
	{
	public:
		ProjectionArrowActor(class Game* game);

		void Input(struct InputState& inputState) override;

	private:
		class MeshRenderer* m_Mesh;
	};
}