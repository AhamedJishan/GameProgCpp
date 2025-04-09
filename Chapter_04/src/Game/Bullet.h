#pragma once

#include "Actor.h"

namespace jLab
{
	class Bullet : public Actor
	{
	public:
		Bullet(class Game* game);
		void UpdateActor(float deltaTime) override;
	private:
		class CircleComponent* m_Circle;
		float m_LiveTime;
	};
}