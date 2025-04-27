#include "Actor.h"

#include "Game.h"
#include "Component/Component.h"

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

		while (!m_Components.empty())
			delete m_Components.back();
	}
	
	void Actor::ProcessInput(const uint8_t* keyState)
	{
		if (m_State == State::E_Active)
		{
			for (Component* component : m_Components)
				component->ProcessInput(keyState);

			InputActor(keyState);
		}
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::E_Active)
		{
			ComputeWorldTransform();

			for (Component* component : m_Components)
				component->Update(deltaTime);

			UpdateActor(deltaTime);

			ComputeWorldTransform();
		}
	}

	void Actor::AddComponent(Component* component)
	{
		int updateOrder = component->GetUpdateOrder();
		auto iter = m_Components.begin();

		for (; iter != m_Components.end(); iter++)
			if (updateOrder < (*iter)->GetUpdateOrder())
				break;

		m_Components.insert(iter, component);
	}

	void Actor::RemoveComponent(Component* component)
	{
		auto iter = std::find(m_Components.begin(), m_Components.end(), component);

		if (iter != m_Components.end())
			m_Components.erase(iter);
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

			for (Component* component : m_Components)
				component->OnUpdateWorldTransform();
		}
	}
}