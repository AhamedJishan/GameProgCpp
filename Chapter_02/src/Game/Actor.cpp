#include "Actor.h"

namespace ch2
{
	Actor::Actor(Game* game)
		: m_Game(game),
		m_State(EActive),
		m_Position(Vector2::Zero),
		m_Rotation(0.0f),
		m_Scale(1.0f)
	{
		m_Game->AddActor(this);
	}
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);

		while (!m_Components.empty())
			delete m_Components.back();
	}

	void Actor::Update(float deltaTime)
	{
		UpdateComponent(deltaTime);
		UpdateActor(deltaTime);
	}
	
	void Actor::UpdateComponent(float deltaTime)
	{
		for (auto component : m_Components)
			component->Update(deltaTime);
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::AddComponent(Component* component)
	{
		int myOrder = component->GetUpdateOrder();
		auto it = m_Components.begin();

		for (; it != m_Components.end(); it++)
		{
			if (myOrder < (*it)->GetUpdateOrder())
				break;
		}

		m_Components.insert(it, component);
	}

	void Actor::RemoveComponent(Component* component)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), component);
		if (it != m_Components.end())
		{
			m_Components.erase(it);
		}
	}
}
