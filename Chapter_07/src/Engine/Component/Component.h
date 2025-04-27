#pragma once

#include <cstdint>

namespace jLab
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void ProcessInput(const uint8_t* keyState);
		virtual void Update(float deltaTime);

		virtual void OnUpdateWorldTransform();

		int GetUpdateOrder() { return m_UpdateOrder; }
		class Actor* GetOwner() { return m_Owner; }

	protected:
		class Actor* m_Owner;
		int m_UpdateOrder;
	};
}