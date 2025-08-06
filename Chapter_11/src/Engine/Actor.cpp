#include "Actor.h"

#include "Component/Component.h"
#include "Game.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game)
		,m_Position(glm::vec3(0))
		,m_Scale(glm::vec3(1))
		,m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)))
		,m_RecomputeWorldTransform(true)
		,m_State(E_Active)
	{
		m_Game->AddActor(this);
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);
	}
	
	void Actor::ComputeWorldTransform()
	{
		if (m_RecomputeWorldTransform)
		{
			m_RecomputeWorldTransform = false;

			m_WorldTransform = glm::scale(glm::mat4(1), m_Scale);
			m_WorldTransform = glm::mat4_cast(m_Rotation) * m_WorldTransform;
			m_WorldTransform = glm::translate(glm::mat4(1), m_Position) * m_WorldTransform;

			for (Component* component : m_Components)
				component->OnUpdateWorldTransform();
		}
	}

	void Actor::LookAt(glm::vec3 direction)
	{
		glm::vec3 dir = glm::normalize(direction);
		float dot = glm::dot(direction, glm::vec3(0, 0, -1));
		float angle = glm::acos(dot);

		if (dot > 0.9999f) SetRotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)));
		else if (dot < -0.9999f) SetRotation(glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)));
		else
		{
			glm::vec3 axisOfRotation = glm::cross(glm::vec3(0, 0, -1), dir);
			axisOfRotation = glm::normalize(axisOfRotation);
			SetRotation(glm::angleAxis(angle, axisOfRotation));
		}
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
		if (m_State == E_Active)
		{
			ComputeWorldTransform();

			for (Component* component : m_Components)
				component->Update(deltaTime);
			Update(deltaTime);

			ComputeWorldTransform();
		}
	}
	
	void Actor::InputActor(const InputState& inputState)
	{
		if (m_State = E_Active)
		{
			for (Component* component : m_Components)
				component->Input(inputState);
			Input(inputState);
		}
	}

	void Actor::AddComponent(Component* component)
	{
		int updateOrder = component->GetUpdateOrder();
		auto iter = m_Components.begin();

		for ( ; iter != m_Components.end(); iter++)
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
	
	void Actor::Update(float deltaTime)
	{
	}
	void Actor::Input(const InputState& inputState)
	{
	}
}