#pragma once

/**
* ---------------------------------------------------------------------------------------------------------
*												Game Class
* ---------------------------------------------------------------------------------------------------------
* The Game class encapsulates the main functionality of an SDL-based game.
* It handles initialization, the main game loop, and cleanup operations.
*
* - Constructor:
*   Initializes `m_IsRunning` (game running state) and `m_TicksCount` (used for delta time calculation).
* 
*
* - Core Methods:
*   1. `init()`:
*      - Initializes SDL with video and audio subsystems.
*      - Creates and stores the main window and renderer.
*      - Initializes the SDL_image library for texture handling.
*
*   2. `runLoop()`:
*      - The main game loop. Continuously runs while `m_IsRunning` is true.
*      - Iteratively calls:
*         - `processInput()`: Handles event polling and user input.
*         - `updateGame()`: Updates game state:-
*							- Calculates `deltaTime` (time elapsed since the last frame).
*							- Updates all active actors in `m_Actors`.
*							- Moves newly added actors from `m_PendingActors` to `m_Actors`.
*							- Removes and deletes "dead" actors from `m_Actors`.
*         - `generateOutput()`: Renders the current frame (clears, generates, and swaps the buffer).
*
*   3. `shutdown()`:
*      - Cleans up SDL components: destroys the renderer and window, and quits SDL subsystems.
* 
*
* - Utility Method:
*   1. `getTexture(const char* filename)`:
*		- If the texture is already in `m_Textures`, it simple returns the texture.
*		- Otherwise it loads an image file stores it in `m_Texture` and then returns it.
* 
*	2. Actor Management:
*		- `addActor(Actor* actor)`:
*			- Adds a new actor to the game. If the game is updating, the actor is stored in `m_PendingActors`
*			and will be moved to `m_Actors` at the end of the current update.
*		- `removeActor(Actor* actor)`: 
*			- Removes an actor from the game. If the game is updating, the actor is marked for removal and 
*			  deleted after the current update.
* 
*	3. Sprite Management:
*		- `addSprite(SpriteComponent* sprite)`:
*			- Adds a new sprite to the game. `m_Sprites` is maintained in an order such that sprites with
*			  lower `drawOrder` values are rendered in front, ensuring correct visual layering.
*		- `removeSprite(SpriteComponent* sprite)`:
*			- Removes a sprite from the game.
* ------------------------------------------------------------------------------------------------------------------
*/


#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace ch3
{
	class Game
	{
	public:
		Game();

		bool init();
		void runLoop();
		void shutdown();

		void addActor(class Actor* actor);
		void removeActor(class Actor* actor);

		void addSprite(class SpriteComponent* sprite);
		void removeSprite(class SpriteComponent* sprite);

		void loadData();
		void unloadData();

		SDL_Texture* getTexture(const char* filename);

		const std::vector<class Asteroid*>& getAsteroids() const { return m_Asteroids; }

		void addAsteroid(class Asteroid* asteroid);
		void removeAsteroid(class Asteroid* asteroid);

	private:
		void processInput();
		void updateGame();
		void generateOutput();

	private:
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

		std::vector<class Actor*> m_Actors;
		std::vector<class Actor*> m_PendingActors;
		bool m_UpdatingActors;

		std::vector<class SpriteComponent*> m_Sprites;

		std::unordered_map<const char*, SDL_Texture*> m_Textures;

		bool m_IsRunnning;
		Uint32 m_TicksCount;

		// Just for this project
		std::vector<class Asteroid*> m_Asteroids;
	};
}