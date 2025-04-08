#pragma once

#include <string>
#include <unordered_map>

#include "Component.h"

namespace jLab
{
	class AIComponent : public Component
	{
	public:
		AIComponent(class Actor* owner);

		void Update(float deltaTime);
		void ChangeState(const std::string& stateName);

		void RegisterState(class AIState* state);
	private:
		std::unordered_map<std::string, class AIState*> m_StateMap;
		class AIState* m_CurrentState;
	};
}