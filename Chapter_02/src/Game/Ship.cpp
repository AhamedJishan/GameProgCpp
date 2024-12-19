#include "Ship.h"

#include "AnimSpriteComponent.h"

namespace ch2
{
	Ship::Ship(Game* game)
		:Actor(game),
		m_SpeedX(0.0f),
		m_SpeedY(0.0f)
	{
		AnimSpriteComponent* asc = new AnimSpriteComponent(this);
		std::vector<SDL_Texture*> textures = {
			game->GetTexture("Assets/Ship01.png"),
			game->GetTexture("Assets/Ship02.png"),
			game->GetTexture("Assets/Ship03.png"),
			game->GetTexture("Assets/Ship04.png"),
		};
		asc->SetAnimTextures(textures);
	}

	void Ship::UpdateActor(float deltaTime)
	{
		Vector2 pos = GetPosition();
		pos.x += m_SpeedX * deltaTime;
		pos.y += m_SpeedY * deltaTime;

		// Restrict the position of the ship
		pos.x = pos.x < 0 ? 0 : pos.x;
		if (pos.x < 25.0f)
			pos.x = 25.0f;
		else if (pos.x > 1280.0f * 0.5f)
			pos.x = 1280.0f * 0.5f;
		if (pos.y < 25.0f)
			pos.y = 25.0f;
		else if (pos.y > 700.0f)
			pos.y = 700.0f;

		SetPosition(pos);
	}

	void Ship::ProcessInput(const uint8_t* state)
	{
		m_SpeedX = 0.0f;
		m_SpeedY = 0.0f;

		if (state[SDL_SCANCODE_W])
			m_SpeedY = -300.0f;
		if (state[SDL_SCANCODE_S])
			m_SpeedY = 300.0f;
		if (state[SDL_SCANCODE_A])
			m_SpeedX = -250.0f;
		if (state[SDL_SCANCODE_D])
			m_SpeedX = 250.0f;
	}
}