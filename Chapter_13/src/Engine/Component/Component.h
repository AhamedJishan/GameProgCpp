#pragma once

#include <cstdint>

namespace jLab
{
	class Component
	{
	public:
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();

		virtual void Update(float deltaTime);
		virtual void Input(const uint8_t* inputState);

		virtual void OnUpdateWorldTransform();

		int GetUpdateOrder() const { return mUpdateOrder; }
		class Actor* GetOwner() const { return mOwner; }

	protected:
		int mUpdateOrder;
		class Actor* mOwner;
	};
}