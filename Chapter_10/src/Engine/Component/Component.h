#pragma once

#include <cstdint>

namespace jLab
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder);
		virtual ~Component();

		virtual void Update(float deltaTime);
		virtual void Input(const uint8_t* keyState);

		virtual void OnUpdateWorldTransform();

		class Actor* GetOwner() const { return m_Owner; }
		int GetUpdateOrder() const { return m_UpdateOrder; }

	private:
		int m_UpdateOrder;
		class Actor* m_Owner;
	};
}