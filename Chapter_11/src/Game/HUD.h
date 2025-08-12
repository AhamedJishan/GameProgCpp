#pragma once

#include "Engine/UIScreen.h"
#include <vector>

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
		class Actor* m_FPSActor;

		class Texture* m_CrosshairWhite;
		class Texture* m_CrosshairRed;

		std::vector<class Actor*> m_Targets;

		bool m_TargetInSight;
	};
}