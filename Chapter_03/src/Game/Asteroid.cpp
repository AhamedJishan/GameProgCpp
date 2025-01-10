#include "Asteroid.h"

#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Random.h"

namespace ch3
{
	Asteroid::Asteroid(Game* game)
		:Actor(game)
	{
		Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1920, 1080));
		float randRot = Random::GetFloatRange(0, Math::TwoPi);

		setPosition(randPos);
		setRotation(randRot);

		MoveComponent* mc = new MoveComponent(this);
		mc->setForwardSpeed(150);

		SpriteComponent* sc = new SpriteComponent(this);
		sc->setTexture(game->getTexture("Assets/Asteroid.png"));

	}

	Asteroid::~Asteroid()
	{
	}
}