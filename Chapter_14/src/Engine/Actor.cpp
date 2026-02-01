#include "Actor.h"

#include "Game.h"
#include "InputSystem.h"
#include "Component/Component.h"
#include "LevelLoader.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:mState(State::Active)
		,mGame(game)
		,mPosition(glm::vec3(0))
		,mScale(glm::vec3(1))
		,mRotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)))
		,mRecomputeWorldTransform(true)
	{
		ComputeWorldTransform();
		mGame->AddActor(this);
	}

	Actor::~Actor()
	{
		mGame->RemoveActor(this);
	}

	void Actor::AddComponent(Component* component)
	{
		int updateOrder = component->GetUpdateOrder();
		auto it = mComponents.begin();
		for (; it != mComponents.end(); it++)
		{
			if ((*it)->GetUpdateOrder() > updateOrder)
				break;
		}
		mComponents.insert(it, component);
	}

	void Actor::RemoveComponent(Component* component)
	{
		auto it = std::find(mComponents.begin(), mComponents.end(), component);
		if (it != mComponents.end())
			mComponents.erase(it);
	}
	
	void Actor::LookAt(const glm::vec3& direction)
	{
		glm::vec3 dir = glm::normalize(direction);
		float dot = glm::dot(dir, glm::vec3(0, 0, -1));
		float angle = glm::acos(dot);

		if (dot > 0.99999f) SetRotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)));
		else if (dot < -0.99999f) SetRotation(glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)));
		else
		{
			glm::vec3 axisOfRotation = glm::cross(dir, glm::vec3(0, 0, -1));
			axisOfRotation = glm::normalize(axisOfRotation);
			SetRotation(glm::angleAxis(angle, axisOfRotation));
		}
	}

	void Actor::LoadProperties(const rapidjson::Value& inObj)
	{
		std::string state;
		if (LevelLoader::GetString(inObj, "state", state))
		{
			if (state == "Active")
				SetState(State::Active);
			else if (state == "Paused")
				SetState(State::Paused);
			else if (state == "Dead")
				SetState(State::Dead);
		}

		LevelLoader::GetVec3(inObj, "position", mPosition);
		LevelLoader::GetVec3(inObj, "scale", mScale);
		LevelLoader::GetQuat(inObj, "rotation", mRotation);

		mRecomputeWorldTransform = true;
		ComputeWorldTransform();
	}

	void Actor::ProcessUpdate(float deltaTime)
	{
		if (mState == State::Active)
		{
			ComputeWorldTransform();

			for (Component* component : mComponents)
				component->Update(deltaTime);

			Update(deltaTime);

			ComputeWorldTransform();
		}
	}
	
	void Actor::ProcessInput(const InputState inputState)
	{
		if (mState == State::Active)
		{
			for (Component* component : mComponents)
				component->Input(inputState);

			Input(inputState);
		}
	}

	void Actor::ComputeWorldTransform()
	{
		if (mRecomputeWorldTransform)
		{
			mRecomputeWorldTransform = false;
			mWorldTransform = glm::scale(glm::mat4(1), mScale);
			mWorldTransform = glm::mat4_cast(mRotation) * mWorldTransform;
			mWorldTransform = glm::translate(glm::mat4(1), mPosition) * mWorldTransform;
			
			for (Component* component : mComponents)
				component->OnUpdateWorldTransform();
		}
	}
	
	void Actor::Update(float deltaTime)
	{
	}
	void Actor::Input(InputState inputState)
	{
	}
}