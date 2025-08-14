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
		void Draw(class Shader* shader) override;

	private:
		void UpdateCrosshair(float deltaTime);
		void UpdateRadar(float deltaTime);

	private:
		class Actor* m_FPSActor;

		class Texture* m_CrosshairWhite;
		class Texture* m_CrosshairRed;
		class Texture* m_Radar;
		class Texture* m_Blip;

		std::vector<class Actor*> m_Targets;
		std::vector<glm::vec2> m_RadarBlips;

		bool m_TargetInSight;
		float m_RadarRange;
		float m_RadarRadius;
	};
}