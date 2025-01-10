#pragma once

/**
* ---------------------------------------------------------------------------------------------------------
*											SpriteComponent Class
* ---------------------------------------------------------------------------------------------------------
* The `SpriteComponent` class provides functionality for rendering 2D textures associated with an `Actor`.
* It calculates the position, scale, and rotation of the texture based on the owning `Actor`.
*
* - Constructor `SpriteComponent(Actor* owner, int drawOrder = 100)`:
*	- Calls the base class constructor `Component`.
*	- Initializes `m_DrawOrder`, which determines the rendering order (lower values are drawn first).
*	- Initializes member variables `m_Texture`, `m_TexWidth`, and `m_TexHeight` to default values.
*	- Registers itself with the game by calling `m_Owner->getGame()->addSprite(this)`.
*
* - Destructor:
*	- Unregisters itself from the game by calling `m_Owner->getGame()->removeSprite(this)`.
*
* - Core Methods:
*	1. `setTexture(SDL_Texture* texture)`:
*		- Assigns the given texture to `m_Texture`.
*		- Uses `SDL_QueryTexture` to retrieve and store the texture's width and height in `m_TexWidth` and `m_TexHeight`.
*
*	2. `draw(SDL_Renderer* renderer)`:
*		- Renders the stored texture (`m_Texture`) on the screen.
*		- Considers the owning `Actor`'s position, scale, and rotation to calculate the rendering rectangle.
*		- Uses `SDL_RenderCopyEx` for rendering with rotation.
*
* - Member Variables:
*   - `m_Texture`: The SDL texture to be rendered.
*   - `m_DrawOrder`: Determines the rendering priority of the sprite.
*   - `m_TexWidth` and `m_TexHeight`: Store the dimensions of the texture.
* ---------------------------------------------------------------------------------------------------------
*/


#include <SDL/SDL.h>

#include "Component.h"

namespace ch3
{
	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(class Actor* owner, int drawOrder = 100);
		~SpriteComponent();

		void draw(SDL_Renderer* renderer);
		void setTexture(SDL_Texture* texture);

		int getDrawOrder() const { return m_DrawOrder; }

	private:
		SDL_Texture* m_Texture;
		int m_DrawOrder;
		int m_TexWidth;
		int m_TexHeight;
	};
}