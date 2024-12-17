#pragma once

namespace ch2
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void Update(float deltaTime);

		int GetUpdateOrder() { return m_UpdateOrder; }

	protected:
		Actor* m_Owner;
		int m_UpdateOrder;
	};
}