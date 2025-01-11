#include "Laser.h"

#include "SpriteComponent.h"
#include "CircleComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Asteroid.h"

namespace ch3
{
	Laser::Laser(Game* game)
		:Actor(game),
		m_DeathTimer(1.0f)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->setTexture(game->getTexture("Assets/Laser.png"));

		MoveComponent* mc = new MoveComponent(this);
		mc->setForwardSpeed(800);

		m_Circle = new CircleComponent(this);
		m_Circle->setRadius(11);

	}

	Laser::~Laser()
	{
	}

	void Laser::updateActor(float deltaTime)
	{
		m_DeathTimer -= deltaTime;
		if (m_DeathTimer <= 0.0f)
		{
			setState(EDead);
			return;
		}

		for (auto asteroid : getGame()->getAsteroids())
		{
			if (intersect(*m_Circle, *(asteroid->getCircle())))
			{
				setState(EDead);
				asteroid->setState(EDead);
			}
		}
	}
}