#pragma once

namespace jLab
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder);
		virtual ~Component();

		virtual void Update(float deltaTime);
		virtual void Input(const struct InputState& inputState);

		virtual void OnUpdateWorldTransform();

		class Actor* GetOwner() const { return m_Owner; }
		int GetUpdateOrder() const { return m_UpdateOrder; }

	protected:
		class Actor* m_Owner;
		int m_UpdateOrder;
	};
}