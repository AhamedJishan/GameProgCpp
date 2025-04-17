#include "Asteroid.h"

#include "Engine/MoveComponent.h"
#include "Engine/SpriteComponent.h"
#include "Engine/CircleComponent.h"
#include "Engine/Game.h"
#include "Engine/util/Math.h"
#include <random>

namespace jLab
{
	Asteroid::Asteroid(Game* game)
		:Actor(game),
		m_Circle(nullptr)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> xDist(-640, 640);
		std::uniform_int_distribution<> yDist(-360, 360);
		std::uniform_int_distribution<> rotDist(0, 360);

		Vector3 randPos = Vector3(xDist(gen), yDist(gen), 1);
		float randRotation = rotDist(gen);

		Actor::SetPosition(randPos);
		Actor::SetRotation(randRotation);

		SpriteComponent* sc = new SpriteComponent(this);
		sc->SetTexture(GetGame()->GetTexture("Assets/Asteroid.png"));

		MoveComponent* mc = new MoveComponent(this);
		mc->SetForwardSpeed(150);

		m_Circle = new CircleComponent(this);
		m_Circle->SetRadius(40);

		GetGame()->AddAsteroid(this);
	}
	
	Asteroid::~Asteroid()
	{
		GetGame()->RemoveAsteroid(this);
	}
}