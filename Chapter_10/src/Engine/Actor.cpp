#include "Actor.h"

#include "Game.h"
#include "Component/Component.h"
#include "InputSystem.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game),
		m_Position(glm::vec3(0)),
		m_Scale(glm::vec3(1)),
		m_State(E_Active),
		m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0))),
		m_RecomputeWorldTransform(true)
	{
		m_Game->AddActor(this);
		ComputeWorldTransform();
	}
	
	Actor::~Actor()
	{
		m_Game->RemoveActor(this);

		while (!m_Components.empty())
			delete m_Components.back();
	}
	
	void Actor::Update(float deltaTime)
	{
		if (m_State == E_Active)
		{
			ComputeWorldTransform();

			ActorUpdate(deltaTime);
			for (Component* component : m_Components)
				component->Update(deltaTime);

			ComputeWorldTransform();
		}
	}
	
	void Actor::ProcessInput(InputState& inputState)
	{
		if (m_State == E_Active)
		{
			ActorInput(inputState);

			for (Component* component : m_Components)
				component->Input(inputState);
		}
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

	void Actor::AddComponent(Component* component)
	{
		int updateOrder = component->GetUpdateOrder();
		auto iter = m_Components.begin();
		for (; iter != m_Components.end(); iter++)
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

	void Actor::LookAt(const glm::vec3& direction)
	{
		glm::vec3 dir = glm::normalize(direction);
		float dot = glm::dot(dir, glm::vec3(0, 0, -1));
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

	void Actor::ActorUpdate(float deltaTime)
	{
	}
	
	void Actor::ActorInput(InputState& inputState)
	{
	}
}