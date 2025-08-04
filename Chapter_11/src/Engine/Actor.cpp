#include "Actor.h"

namespace jLab
{
	Actor::Actor(Game* game)
		:m_Game(game)
		,m_Position(glm::vec3(0))
		,m_Scale(glm::vec3(1))
		,m_Rotation(glm::angleAxis(0.0f, glm::vec3(0, 1, 0)))
		,m_RecomputeWorldTransform(true)
		,m_State(E_Active)
	{
		// TODO: add this actor to game
	}
	
	Actor::~Actor()
	{
		// TODO: remove this actor from game
	}
	
	void Actor::ComputeWorldTransform()
	{
	}
	
	void Actor::UpdateActor(float deltaTime)
	{
	}
	
	void Actor::InputActor(const InputState& inputState)
	{
	}
	
	void Actor::Update(float deltaTime)
	{
	}
	void Actor::Input(const InputState& inputState)
	{
	}
}