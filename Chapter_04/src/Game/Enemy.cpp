#include "Enemy.h"

#include "Game.h"
#include "Grid.h"
#include "Tile.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "CircleComponent.h"
#include <algorithm>

namespace jLab
{
	Enemy::Enemy(Game* game)
		:Actor(game),
		m_Circle(nullptr)
	{
		game->GetEnemies().emplace_back(this);

		SpriteComponent* sc = new SpriteComponent(this, 200);
		sc->SetTexture(game->GetTexture("Assets/Airplane.png"));

		SetPosition(game->GetGrid()->GetStartTile()->GetPosition());

		NavComponent* nc = new NavComponent(this);
		nc->SetForwardSpeed(150);
		nc->StartPath(game->GetGrid()->GetStartTile());

		m_Circle = new CircleComponent(this);
		m_Circle->SetRadius(25);
	}
	
	Enemy::~Enemy()
	{
		auto iter = std::find(GetGame()->GetEnemies().begin(), GetGame()->GetEnemies().end(), this);
		GetGame()->GetEnemies().erase(iter);
	}
	
	void Enemy::UpdateActor(float deltaTime)
	{
		Actor::UpdateActor(deltaTime);

		Vector2 diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();
		if (diff.Length() < 10.0f)
			SetState(Actor::EDead);
	}
}