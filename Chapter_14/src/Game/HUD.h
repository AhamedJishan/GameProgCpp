#pragma once

#include "Engine/UIScreen.h"
#include "Engine/Shader.h"
#include <vector>
#include <glm/vec2.hpp>

namespace jLab
{
	class HUD : public UIScreen
	{
	public:
		HUD(class Game* game, class Actor* fpsActor, std::vector<class Actor*>& targets);
		~HUD();

		void Update(float deltaTime) override;
		void Draw(const class Shader* shader) override;

	private:
		void UpdateCrosshair(float deltaTime);
		void UpdateRadar(float deltaTime);

	private:
		class Actor* mFPSActor;

		class Texture* mCrosshairWhite;
		class Texture* mCrosshairRed;
		class Texture* mRadar;
		class Texture* mBlip;

		std::vector<class Actor*> mTargets;
		std::vector<glm::vec2> mRadarBlips;

		bool mTargetInSight;
		float mRadarRange;
		float mRadarRadius;
	};
}