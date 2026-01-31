#include "Component.h"

#include "Engine/InputSystem.h"
#include "Engine/Actor.h"
#include "Engine/LevelLoader.h"

namespace jLab
{
	Component::Component(Actor* owner, int updateOrder)
		:mOwner(owner)
		,mUpdateOrder(updateOrder)
	{
		mOwner->AddComponent(this);
	}

	Component::~Component()
	{
		mOwner->RemoveComponent(this);
	}

	void Component::Update(float deltaTime)
	{
	}

	void Component::Input(const InputState inputState)
	{
	}

	void Component::OnUpdateWorldTransform()
	{
	}

	void Component::LoadProperties(const rapidjson::Value& inObj)
	{
		LevelLoader::GetInt(inObj, "updateOrder", mUpdateOrder);
	}
}