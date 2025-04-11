#include "Engine/Game.h"
#include <iostream>

using namespace jLab;

int main(int argc, char* argv[])
{
	Game game;
	if (game.Init())
		game.Run();
	game.ShutDown();

	return 0;
}