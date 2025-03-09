#include "Game/Game.h"

using namespace jLab;

int main(int argc, char* argv[])
{
	Game game;
	if (game.Init())
		game.RunLoop();
	game.Shutdown();

	return 0;
}