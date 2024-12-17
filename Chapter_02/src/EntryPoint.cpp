#include <iostream>

#include "Game/Game.h"

using namespace ch2;

int main(int argc, char* argv[])
{
	Game game;
	game.Init();
	game.RunLoop();
	game.Shutdown();

	return 0;
}