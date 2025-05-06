#include "Actor.h"

#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_Position(glm::vec3(0)),
		m_Scale(glm::vec3(1)),
		m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0))),
		m_RecomputeTransform(true)
	{
		m_Game->AddActor(this);
		ComputeTransform();
	}

	Actor::~Actor()
	{
		m_Game->RemoveActor(this);
		// TODO: delete all components
	}
	
	void Actor::ProcessInput(InputState& inputState)
	{
		if (m_State == E_Active)
		{
			// TODO: Input for all components
			Input(inputState);
		}
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
		if (m_State == E_Active)
		{
			ComputeTransform();

			// TODO: Update all components
			Update(deltaTime);

			ComputeTransform();
		}
	}

	void Actor::ComputeTransform()
	{
		if (m_RecomputeTransform)
		{
			m_RecomputeTransform = false;

			m_WorldTransform = glm::translate(glm::mat4(1), m_Position);
			m_WorldTransform = glm::scale(m_WorldTransform, m_Scale);
			m_WorldTransform = m_WorldTransform * glm::mat4_cast(m_Rotation);

			// TODO: OnUpdateWorldTransform for all components
		}
	}
	
	// Virtual functions
	void Actor::Input(InputState& inputState)
	{
	}
	void Actor::Update(float deltaTime)
	{
	}
	
}