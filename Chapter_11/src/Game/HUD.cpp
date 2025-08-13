#include "HUD.h"

#include <glm/vec3.hpp>
#include <glm/mat2x2.hpp>
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
		,m_RadarRadius(120.0f * 0.75f)
		,m_RadarRange(12.5f)
	{
		m_CrosshairRed = m_Game->GetRenderer()->GetTexture("Assets/Textures/CrosshairRed.png");
		m_CrosshairWhite = m_Game->GetRenderer()->GetTexture("Assets/Textures/Crosshair.png");
		m_Radar = m_Game->GetRenderer()->GetTexture("Assets/Textures/Radar.png");
		m_Blip = m_Game->GetRenderer()->GetTexture("Assets/Textures/Blip.png");
	}

	HUD::~HUD()
	{
	}
	
	void HUD::Update(float deltaTime)
	{
		UIScreen::Update(deltaTime);

		UpdateCrosshair(deltaTime);
		UpdateRadar(deltaTime);
	}
	
	void HUD::Draw(Shader* shader)
	{
		UIScreen::Draw(shader);

		// Draw Crosshair
		Texture* texture = m_TargetInSight ? m_CrosshairRed : m_CrosshairWhite;
		DrawTexture(shader, texture, glm::vec3(0));

		// Draw Radar
		glm::vec2 radarPos = glm::vec2(-512, 232);
		DrawTexture(shader, m_Radar, radarPos, glm::vec2(0.75f));
		for (glm::vec2& pos : m_RadarBlips)
			DrawTexture(shader, m_Blip, radarPos + pos);
	}
	void HUD::UpdateCrosshair(float deltaTime)
	{
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

	void HUD::UpdateRadar(float deltaTime)
	{
		m_RadarBlips.clear();

		glm::vec3 playerPos = m_FPSActor->GetPosition();
		glm::vec3 playerForward = m_FPSActor->GetForward();

		glm::vec2 playerPos2D = glm::vec2(playerPos.x, -playerPos.z);
		glm::vec2 playerForward2D = glm::vec2(playerForward.x, -playerForward.z);

		float angle = std::atan2f(playerForward2D.x, playerForward2D.y);
		glm::mat3 mat3Rot= glm::mat3_cast(glm::angleAxis(angle, glm::vec3(0, 0, 1)));
		glm::mat2 rotation(
			mat3Rot[0][0], mat3Rot[0][1],
			mat3Rot[1][0], mat3Rot[1][1]
		);

		for (Actor* target : m_Targets)
		{
			glm::vec3 targetPos = target->GetPosition();
			glm::vec2 targetPos2D = glm::vec2(targetPos.x, -targetPos.z);

			glm::vec2 playerToTarget = targetPos2D - playerPos2D;

			if (glm::length(playerToTarget) <= m_RadarRange)
			{
				glm::vec2 blipPos = playerToTarget;
				blipPos *= m_RadarRadius / m_RadarRange;

				blipPos = rotation * blipPos;
				m_RadarBlips.emplace_back(blipPos);
			}
		}
	}
}