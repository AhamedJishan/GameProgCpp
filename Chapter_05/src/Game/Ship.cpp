#include "Ship.h"

#include "Engine/InputComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/Game.h"
#include "Engine/util/Math.h"
#include "Laser.h"

namespace jLab
{
	Ship::Ship(Game* game)
		:Actor(game),
		m_LaserCooldown(0)
	{
		SpriteComponent* sc = new SpriteComponent(this, 200);
		sc->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));

		InputComponent* ic = new InputComponent(this);
		ic->SetMaxForwardSpeed(300);
		ic->SetMaxAngularSpeed(Math::TwoPi);
	}
	
	void Ship::UpdateActor(float deltaTime)
	{
		m_LaserCooldown -= deltaTime;
	}
	
	void Ship::ActorInput(const uint8_t* keyState)
	{
		if (keyState[SDL_SCANCODE_SPACE] && m_LaserCooldown <= 0)
		{
			Laser* laser = new Laser(GetGame());
			laser->SetPosition(GetPosition());
			laser->SetRotation(GetRotation());

			m_LaserCooldown = 0.5f;
		}
	}
}