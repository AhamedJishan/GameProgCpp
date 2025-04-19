#include "Actor.h"

#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_Position(glm::vec3(0, 0, 0)),
		m_Rotation(glm::quat(0, glm::vec3(0, 0, 1))),
		m_Scale(glm::vec3(1, 1, 1)),
		m_State(State::EActive),
		m_RecomputeWorldTransform(true)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);
	}
	
	void Actor::ProcessInput(const uint8_t* keyState)
	{
		if (m_State == State::EActive)
		{
			// TODO: Process Input for all components
			InputActor(keyState);
		}
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::EActive)
		{
			CalculateWorldTransform();

			// TODO: Update all components
			UpdateActor(deltaTime);

			CalculateWorldTransform();
		}
	}
	
	// Virtual
	void Actor::UpdateActor(float deltaTime)
	{
	}
	// Virtual
	void Actor::InputActor(const uint8_t* keyState)
	{
	}
	
	void Actor::CalculateWorldTransform()
	{
		if (m_RecomputeWorldTransform)
		{
			m_RecomputeWorldTransform = false;

			glm::mat4 world = glm::mat4(1.0f);
			world = glm::scale(world, m_Scale);
			world = glm::mat4_cast(m_Rotation) * world;
			world = glm::translate(world, m_Position);

			m_WorldTransform = world;

			// TODO: call OnWorldTransform() for all componenets
		}
	}
}