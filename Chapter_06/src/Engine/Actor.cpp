#include "Actor.h"

#include "Game.h"
#include "Component/Component.h"
#include <algorithm>

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_Position(glm::vec3(0, 0, 0)),
		m_Rotation(glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0))),
		m_Scale(glm::vec3(1, 1, 1)),
		m_State(State::EActive),
		m_RecomputeWorldTransform(true)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);

		// ~Component() removes itself from m_Components
		while (!m_Components.empty())
			delete m_Components.back();
	}
	
	void Actor::ProcessInput(const uint8_t* keyState)
	{
		if (m_State == State::EActive)
		{
			for (Component* component : m_Components)
				component->ProcessInput(keyState);

			InputActor(keyState);
		}
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::EActive)
		{
			CalculateWorldTransform();

			for (Component* component : m_Components)
				component->Update(deltaTime);

			UpdateActor(deltaTime);

			CalculateWorldTransform();
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

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_Position);
			model = glm::scale(model, m_Scale);
			model = model * glm::mat4_cast(m_Rotation);

			m_WorldTransform = model;

			for (Component* component : m_Components)
				component->OnWorldTransformUpdate();
		}
	}
}