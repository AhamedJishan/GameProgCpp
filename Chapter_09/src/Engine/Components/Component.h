#pragma once

#include "Engine/InputSystem.h"

namespace jLab
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void Input(const InputState& state);
		virtual void Update(float deltaTime);
		virtual void OnWorldTranformUpdate();

		class Actor* GetOwner() const { return m_Owner; }
		int GetUpdateOrder() const { return m_UpdateOrder; }

	protected:
		class Actor* m_Owner;
		int m_UpdateOrder;
	};
}