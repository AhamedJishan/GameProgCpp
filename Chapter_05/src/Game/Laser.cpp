#include "Laser.h"

#include "Engine/MoveComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/CircleComponent.h"
#include "Engine/Game.h"
#include "Engine/util/Math.h"
#include "Asteroid.h"

namespace jLab
{
	Laser::Laser(Game* game)
		:Actor(game)
		,m_DeathTimer(1),
		m_Circle(nullptr)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->SetTexture(GetGame()->GetTexture("Assets/Laser.png"));

		MoveComponent* mc = new MoveComponent(this);
		mc->SetForwardSpeed(800);

		m_Circle = new CircleComponent(this);
		m_Circle->SetRadius(11);
	}

	void Laser::UpdateActor(float deltaTime)
	{
		m_DeathTimer -= deltaTime;

		if (m_DeathTimer < 0)
		{
			Actor::SetState(Actor::EDead);
		}
		else
		{
			for (Asteroid* asteroid : GetGame()->GetAsteroids())
			{
				if (Intersect(*m_Circle, *(asteroid->GetCircle())))
				{
					asteroid->SetState(Actor::EDead);
					SetState(Actor::EDead);
					break;
				}
			}
		}
	}
}