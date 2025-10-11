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

		void SetObjectBox(const AABB& model) { mObjectBox = model; OnUpdateWorldTransform(); }
		const AABB& GetWorldBox() const { return mWorldBox; }

		void SetShouldRotate(bool shouldRotate) { mShouldRotate = shouldRotate; }

	private:
		AABB mObjectBox;
		AABB mWorldBox;
		bool mShouldRotate;
	};
}