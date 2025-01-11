#include "Ship.h"

#include "Game.h"
#include "SpriteComponent.h"
#include "InputComponent.h"

namespace ch3
{
	Ship::Ship(Game* game)
		: Actor(game)
	{
		SpriteComponent* sc = new SpriteComponent(this);
		sc->setTexture(game->getTexture("Assets/Ship.png"));

		InputComponent* ic = new InputComponent(this);
		ic->setMaxAngularSpeed	(7);
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
}