#include "Tower.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "Enemy.h"
#include "Bullet.h"
#include "util/Math.h"

namespace jLab
{
	Tower::Tower(Game* game)
		:Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this, 200);
		sc->SetTexture(game->GetTexture("Assets/Tower.png"));

		m_NextAttack = m_AttackTime;
	}
	
	void Tower::UpdateActor(float deltaTime)
	{
		Actor::UpdateActor(deltaTime);

		m_NextAttack -= deltaTime;
		if (m_NextAttack <= 0)
		{
			Enemy* enemy = GetGame()->GetNearestEnemy(GetPosition());
			if (enemy != nullptr)
			{
				Vector2 dir = enemy->GetPosition() - GetPosition();
				float dist = dir.Length();
				
				if (dist < m_AttackRange)
				{
					SetRotation(Math::Atan2(-dir.y, dir.x));

					Bullet* bullet = new Bullet(GetGame());
					bullet->SetPosition(GetPosition());
					bullet->SetRotation(GetRotation());
				}
			}
			m_NextAttack += m_AttackTime;
		}
	}
}