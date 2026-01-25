#include "Engine/Game.h"

using namespace jLab;

int main(int argc, char* argv)
{
	Game game;
	if (game.Init())
		game.Run();
	game.Shutdown();

	return 0;
}