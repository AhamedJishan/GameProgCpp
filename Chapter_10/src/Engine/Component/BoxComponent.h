#pragma once

#include "Component.h"
#include "Engine/Collision.h"

namespace jLab
{
	class BoxComponent : public Component
	{
	public:
		BoxComponent(class Actor* owner, int updateOrder = 100);
		~BoxComponent();

		void OnUpdateWorldTransform() override;

		void SetObjectBox(const AABB& model) { m_ObjectBox = model; }
		const AABB& GetWorldBox() const { return m_WorldBox; }

		void SetShouldRotate(bool shouldRotate) { m_ShouldRotate = shouldRotate; }

	private:
		AABB m_ObjectBox;
		AABB m_WorldBox;
		bool m_ShouldRotate;
	};
}