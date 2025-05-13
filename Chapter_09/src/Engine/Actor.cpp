#include "Actor.h"

#include "Game.h"
#include "Components/Component.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_Position(glm::vec3(0)),
		m_Scale(glm::vec3(1)),
		m_State(E_Active),
		m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0))),
		m_RecomputeTransform(true)
	{
		m_Game->AddActor(this);
		ComputeTransform();
	}

	Actor::~Actor()
	{
		m_Game->RemoveActor(this);

		while (!m_Components.empty())
			delete m_Components.back();
	}
	
	void Actor::ProcessInput(InputState& inputState)
	{
		if (m_State == E_Active)
		{
			for (Component* component : m_Components)
				component->Input(inputState);
			Input(inputState);
		}
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
		if (m_State == E_Active)
		{
			ComputeTransform();

			for (Component* component : m_Components)
				component->Update(deltaTime);
			Update(deltaTime);

			ComputeTransform();
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

	void Actor::ComputeTransform()
	{
		if (m_RecomputeTransform)
		{
			m_RecomputeTransform = false;

			m_WorldTransform = glm::translate(glm::mat4(1), m_Position);
			m_WorldTransform = glm::scale(m_WorldTransform, m_Scale);
			m_WorldTransform = m_WorldTransform * glm::mat4_cast(m_Rotation);

			for (Component* component : m_Components)
				component->OnWorldTranformUpdate();
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