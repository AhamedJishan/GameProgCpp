#pragma once

#include "Actor.h"

namespace ch2
{
	class Component
	{
	public:
		Component(Actor* owner, int updateOrder = 100);
		virtual ~Component();

		void Update(float deltaTime);

		int GetUpdateOrder() { return m_UpdateOrder; }

	private:
		Actor* m_Owner;
		int m_UpdateOrder;
	};
}