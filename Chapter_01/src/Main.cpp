#include "Game/Game.h"
#include <iostream>

int main(int argc, char* argv[])
{
	Game game;
	bool success = game.initialise();
	if (success)
		game.runLoop();
	game.shutdown();

	return 0;
}