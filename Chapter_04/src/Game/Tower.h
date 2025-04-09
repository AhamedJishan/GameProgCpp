#pragma once

#include "Actor.h"

namespace jLab
{
	class Tower : public Actor
	{
	public:
		Tower(class Game* game);
		void UpdateActor(float deltaTime) override;

	private:
		float m_NextAttack;
		const float m_AttackTime = 2.5f;
		const float m_AttackRange = 100.0f;
	};
}