#include "Actor.h"

#include "Game.h"
#include "Component.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Position(Vector2::Zero),
		m_Rotation(0),
		m_Scale(1),
		m_Game(game)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == State::EActive)
		{
			UpdateComponent(deltaTime);
			UpdateActor(deltaTime);
		}
	}
	
	void Actor::UpdateComponent(float deltaTime)
	{
		for (Component* component : m_Components)
			component->Update(deltaTime);
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::ProcessInput(const uint8_t* keyboardState)
	{
		for (Component* component : m_Components)
			component->ProcessInput(keyboardState);

		ActorInput(keyboardState);
	}
	
	void Actor::ActorInput(const uint8_t* keyboardState)
	{
	}
	
	void Actor::AddComponent(Component* component)
	{
		int updateOrder = component->GetUpdateOrder();
		auto iter = m_Components.begin();
		for ( ; iter != m_Components.end(); ++iter)
		{
			if (updateOrder < (*iter)->GetUpdateOrder())
				break;
		}
		m_Components.insert(iter, component);
	}
	
	void Actor::RemoveComponent(Component* component)
	{
		auto iter = std::find(m_Components.begin(), m_Components.end(), component);
		if (iter != m_Components.end())
			m_Components.erase(iter);
	}
}