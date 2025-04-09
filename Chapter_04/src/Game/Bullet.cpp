#include "Bullet.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Enemy.h"


namespace jLab
{
	Bullet::Bullet(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this, 210);
		sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

		MoveComponent* mc = new MoveComponent(this);
		mc->SetForwardSpeed(450);

		m_Circle = new CircleComponent(this);
		m_Circle->SetRadius(5);

		m_LiveTime = 1.0f;
	}
	
	void Bullet::UpdateActor(float deltaTime)
	{
		Actor::UpdateActor(deltaTime);

		for (auto enemy : GetGame()->GetEnemies())
		{
			if (Intersect(*(enemy->GetCircle()), *m_Circle))
			{
				enemy->SetState(Actor::EDead);
				SetState(Actor::EDead);
				break;
			}
		}

		m_LiveTime -= deltaTime;
		if (m_LiveTime <= 0)
			SetState(Actor::EDead);
	}
}