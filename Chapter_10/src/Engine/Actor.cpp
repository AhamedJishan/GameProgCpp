#include "Actor.h"

#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		: m_Game(game)
		, m_Position(0, 0, 0)
		, m_Scale (1, 1, 1)
		, m_Rotation(glm::angleAxis(0.0f, glm::vec3(1, 0, 0)))
		, m_RecomputeWorldTransform(true)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);

		// TODO: Remove all the Components
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == E_Active)
		{
			ComputeWorldTransform();

			UpdateActor(deltaTime);
			// TODO: Update Components

			ComputeWorldTransform();
		}
	}
	
	void Actor::ProcessInput(const uint8_t* keyState)
	{
		if (m_State == E_Active)
		{
			ActorInput(keyState);
			// TODO: Input for each component
		}
	}
	
	void Actor::ComputeWorldTransform()
	{
		if (m_RecomputeWorldTransform)
		{
			m_RecomputeWorldTransform = false;

			m_WorldTransform = glm::translate(glm::mat4(1), m_Position);
			m_WorldTransform = glm::mat4_cast(m_Rotation) * m_WorldTransform;
			m_WorldTransform = glm::scale(m_WorldTransform, m_Scale);

			// TODO: Each component OnTransformUpdate()
		}
	}

	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::ActorInput(const uint8_t* keyState)
	{
	}
}