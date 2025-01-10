#pragma once

/**
* ---------------------------------------------------------------------------------------------------------
*											AnimSpriteComponent Class
*								--------------------------------------------------------
*								Component ----> SpriteComponent ----> AnimSpriteComponent
* ---------------------------------------------------------------------------------------------------------
* The AnimSpriteComponent class extends the functionality of SpriteComponent to allow for frame-based
* animated sprites. It cycles through a series of textures at a specified frame rate.
* 
* - Constructor `AnimSpriteComponent(class Actor* actor, int drawOrder = 100, float fps = 24.0f)`:
*	- Calls the super constructor SpriteComponent(actor, drawOrder).
*	- Sets the frame rate (`m_FPS`) and initializes `m_CurrFrame` to 0.
* 
* - Core methods:
*	1. update(float deltaTime):
*		- Overrides the SpriteComponent update method.
*		- Calls the super class version of update method.
*		- Updates the current frame based on the elapsed time and frame rate.
*		- Cycles through the animation textures and updates the current texture in the SpriteComponent.
* 
*	2. setAnimTextures(std::vector<SDL_Texture*> textures):
*		- Sets the textures used for animation.
*		- Resets the current frame and sets the first texture as the default.
* 
* - Getters and Setters for `m_CurrFrame` and `m_FPS`.
* 
* - Member Variables:
*   - `m_AnimTextures`: A vector containing the textures used for animation.
*   - `m_CurrFrame`: The current frame of the animation (incremented every update).
*   - `m_FPS`: The frame rate for the animation (frames per second).
* ---------------------------------------------------------------------------------------------------------
*/

#include "SpriteComponent.h"
#include <vector>

namespace ch3
{
	class AnimSpriteComponent : public SpriteComponent
	{
	public:
		AnimSpriteComponent(class Actor* actor, int drawOrder = 100, float fps = 24.0f);

		void update(float deltaTime) override;

		void setAnimTextures(std::vector<SDL_Texture*> textures);

		float getAnimFPS() const { return m_FPS; }
		void setAnimFPS(float fps) { m_FPS = fps; }

		float getCurrFrame() const { return m_CurrFrame; }
		void setCurrFrame(int frame) { m_FPS = static_cast<float> (frame); }

	private:
		std::vector<SDL_Texture*> m_AnimTextures;

		float m_CurrFrame;
		float m_FPS;
	};
}