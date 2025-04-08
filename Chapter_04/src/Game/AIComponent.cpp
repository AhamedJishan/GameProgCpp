#include "AIComponent.h"

#include <SDL/SDL.h>
#include "AIState.h"

namespace jLab
{
	AIComponent::AIComponent(Actor* owner)
		:Component(owner),
		m_CurrentState(nullptr)
	{
	}
	
	void AIComponent::Update(float deltaTime)
	{
		if (m_CurrentState)
			m_CurrentState->Update(deltaTime);
	}
	
	void AIComponent::ChangeState(const std::string& stateName)
	{
		if (m_CurrentState)
			m_CurrentState->OnExit();

		auto iter = m_StateMap.find(stateName);
		if (iter != m_StateMap.end())
		{
			m_CurrentState = iter->second;
			m_CurrentState->OnEnter();
		}
		else
		{
			SDL_Log("Could not find AIState %s in state map", stateName);
			m_CurrentState = nullptr;
		}
	}
	
	void AIComponent::RegisterState(AIState* state)
	{
		m_StateMap.emplace(state->GetName(), state);
	}
}