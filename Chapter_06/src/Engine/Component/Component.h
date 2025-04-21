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

		class Actor* GetOwner() { return m_Owner; }
		int GetUpdateOrder() { return m_UpdateOrder; }

		virtual void OnWorldTransformUpdate();

	protected:
		class Actor* m_Owner;
		int m_UpdateOrder;
	};
}