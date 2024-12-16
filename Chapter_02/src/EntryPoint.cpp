#include <iostream>

#include "Game/Game.h"

using namespace ch2;

int main(int argc, char* argv[])
{
	Game game;
	game.Init();
	game.RunLoop();
	game.Shutdown();

	std::cout << "Hello!";
	std::cin.get();

	return 0;
}