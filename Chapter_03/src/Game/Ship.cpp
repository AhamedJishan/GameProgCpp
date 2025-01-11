#include "Ship.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Laser.h"

namespace ch3
{
	Ship::Ship(Game* game)
		: Actor(game),
		m_LaserCooldown(0.0f)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->setTexture(game->getTexture("Assets/Ship.png"));

		InputComponent* ic = new InputComponent(this);
		ic->setMaxAngularSpeed	(Math::TwoPi);
		ic->setMaxForwardSpeed	(300);
		ic->setForwardKey		(SDL_SCANCODE_UP);
		ic->setBackKey			(SDL_SCANCODE_DOWN);
		ic->setClockWiseKey		(SDL_SCANCODE_LEFT);
		ic->setAntiClockWiseKey	(SDL_SCANCODE_RIGHT);

		this->setPosition(Vector2(1280/2, 720/2));
	}

	Ship::~Ship()
	{
	}

	void Ship::actorInput(const uint8_t* keyState)
	{
		if (keyState[SDL_SCANCODE_SPACE] && m_LaserCooldown <= 0.0f)
		{
			Laser* laser = new Laser(getGame());
			laser->setPosition(getPosition());
			laser->setRotation(getRotation());

			m_LaserCooldown = 0.5f;
		}
	}

	void Ship::updateActor(float deltaTime)
	{
		m_LaserCooldown -= deltaTime;
	}
}