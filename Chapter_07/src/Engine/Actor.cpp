#include "Actor.h"

#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_State(State::E_Active),
		m_Position(glm::vec3(0)),
		m_Scale(glm::vec3(1)),
		m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0))),
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
		if (m_State == State::E_Active)
		{
			// TODO: Process input for all components
			InputActor(keyState);
		}
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::E_Active)
		{
			ComputeWorldTransform();

			// TODO: Update all components
			UpdateActor(deltaTime);

			ComputeWorldTransform();
		}
	}
	
	void Actor::InputActor(const uint8_t* keyState)
	{
	}
	void Actor::UpdateActor(float deltaTime)
	{
	}

	void Actor::ComputeWorldTransform()
	{
		if (m_RecomputeWorldTransform)
		{
			m_RecomputeWorldTransform = false;

			m_WorldTransform = glm::translate(glm::mat4(1), m_Position);
			m_WorldTransform = glm::scale(m_WorldTransform, m_Scale);
			m_WorldTransform = m_WorldTransform * glm::mat4_cast(m_Rotation);

			// TODO: Let each component know that the transform has been updated
		}
	}
}