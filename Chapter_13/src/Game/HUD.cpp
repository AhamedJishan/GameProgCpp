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
		,mTargets(targets)
		,mFPSActor(fpsActor)
		,mRadarRadius(120.0f * 0.75f)
		,mRadarRange(12.5f)
	{
		mCrosshairRed = mGame->GetRenderer()->GetTexture("Assets/Textures/CrosshairRed.png");
		mCrosshairWhite = mGame->GetRenderer()->GetTexture("Assets/Textures/Crosshair.png");
		mRadar = mGame->GetRenderer()->GetTexture("Assets/Textures/Radar.png");
		mBlip = mGame->GetRenderer()->GetTexture("Assets/Textures/Blip.png");
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
	
	void HUD::Draw(const Shader* shader)
	{
		UIScreen::Draw(shader);

		// Draw Crosshair
		Texture* texture = mTargetInSight ? mCrosshairRed : mCrosshairWhite;
		DrawTexture(shader, texture, glm::vec3(0));

		// Draw Radar
		glm::vec2 radarPos = glm::vec2(-512, 232);
		DrawTexture(shader, mRadar, radarPos, glm::vec2(0.75f));
		for (glm::vec2& pos : mRadarBlips)
			DrawTexture(shader, mBlip, radarPos + pos);

		// Draw Rear view cam
		Texture* rearViewTexture = mGame->GetRenderer()->GetMirrorTexture();
		DrawTexture(shader, rearViewTexture, glm::vec2(-460, -250), glm::vec2(1, -1));
	}
	void HUD::UpdateCrosshair(float deltaTime)
	{
		mTargetInSight = false;

		float lineLength = 5000.0f;
		glm::vec3 start, dir;
		mGame->GetRenderer()->ScreenToWorldDir(start, dir);

		LineSegment line(start, start + dir * lineLength);

		CollisionInfo info;
		if (mGame->GetPhysWorld()->SegmentCast(line, info, mFPSActor) && !mTargets.empty())
		{
			for (Actor* target : mTargets)
			{
				if (info.m_Actor == target)
				{
					mTargetInSight = true;
					break;
				}
			}
		}
	}

	void HUD::UpdateRadar(float deltaTime)
	{
		mRadarBlips.clear();

		glm::vec3 playerPos = mFPSActor->GetPosition();
		glm::vec3 playerForward = mFPSActor->GetForward();

		glm::vec2 playerPos2D = glm::vec2(playerPos.x, -playerPos.z);
		glm::vec2 playerForward2D = glm::vec2(playerForward.x, -playerForward.z);

		float angle = std::atan2f(playerForward2D.x, playerForward2D.y);
		glm::mat3 mat3Rot= glm::mat3_cast(glm::angleAxis(angle, glm::vec3(0, 0, 1)));
		glm::mat2 rotation(
			mat3Rot[0][0], mat3Rot[0][1],
			mat3Rot[1][0], mat3Rot[1][1]
		);

		for (Actor* target : mTargets)
		{
			glm::vec3 targetPos = target->GetPosition();
			glm::vec2 targetPos2D = glm::vec2(targetPos.x, -targetPos.z);

			glm::vec2 playerToTarget = targetPos2D - playerPos2D;

			if (glm::length(playerToTarget) <= mRadarRange)
			{
				glm::vec2 blipPos = playerToTarget;
				blipPos *= mRadarRadius / mRadarRange;

				blipPos = rotation * blipPos;
				mRadarBlips.emplace_back(blipPos);
			}
		}
	}
}