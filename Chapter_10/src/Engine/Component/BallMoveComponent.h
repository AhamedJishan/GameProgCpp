#pragma once

#include "MoveComponent.h"

namespace jLab
{
	class BallMoveComponent : public MoveComponent
	{
	public:
		BallMoveComponent(class Actor* owner, int updateOrder = 100);

		void Update(float deltaTime) override;
		void SetPlayer(class Actor* player) { m_Player = player; }

	private:
		class Actor* m_Player;
	};
}