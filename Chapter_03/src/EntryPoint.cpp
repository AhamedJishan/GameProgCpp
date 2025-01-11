#include <iostream>

#include "Game/Game.h"

using namespace ch3;

int main(int argc, char* argv[])
{
	Game game;
	if (game.init())
		game.runLoop();
	game.shutdown();

	return 0;
}