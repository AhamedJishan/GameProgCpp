#pragma once

namespace jLab
{
	class AIState
	{
	public:
		AIState(class AIComponent* owner)
			:m_Owner(owner)
		{
		}

		virtual void Update(float deltaTime) = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

		virtual const char* GetName() const = 0;

	protected:
		class AIComponent* m_Owner;
	};
}