#pragma once

#include <cstdint>

namespace jLab
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void Update(float deltaTime) {}
		virtual void ProcessInput(const uint8_t* keyboardState) {}

		int GetUpdateOrder() { return m_UpdateOrder; }

	protected:
		int m_UpdateOrder;
		class Actor* m_Owner;
	};
}