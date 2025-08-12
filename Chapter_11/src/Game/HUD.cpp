#include "HUD.h"

#include <glm/vec3.hpp>
#include "Engine/Actor.h"
#include "Engine/Collision.h"
#include "Engine/PhysWorld.h"
#include "Engine/Game.h"
#include "Engine/Renderer.h"
#include "Engine/Actor.h"

namespace jLab
{
	HUD::HUD(Game* game, Actor* fpsActor, std::vector<class Actor*>& targets)
		:UIScreen(game)
		,m_Targets(targets)
		,m_FPSActor(fpsActor)
	{
		m_CrosshairRed = m_Game->GetRenderer()->GetTexture("Assets/Textures/CrosshairRed.png");
		m_CrosshairWhite = m_Game->GetRenderer()->GetTexture("Assets/Textures/Crosshair.png");
	}

	HUD::~HUD()
	{
	}
	
	void HUD::Update(float deltaTime)
	{
		UIScreen::Update(deltaTime);

		m_TargetInSight = false;

		float lineLength = 5000.0f;
		glm::vec3 start, dir;
		m_Game->GetRenderer()->ScreenToWorldDir(start, dir);

		LineSegment line(start, start + dir * lineLength);

		CollisionInfo info;
		if (m_Game->GetPhysWorld()->SegmentCast(line, info, m_FPSActor) && !m_Targets.empty())
		{
			for (Actor* target : m_Targets)
			{
				if (info.m_Actor == target)
				{
					m_TargetInSight = true;
					break;
				}
			}
		}
	}
	
	void HUD::Draw(Shader* shader)
	{
		UIScreen::Draw(shader);

		Texture* texture = m_TargetInSight ? m_CrosshairRed : m_CrosshairWhite;
		DrawTexture(shader, texture, glm::vec3(0));
	}
}