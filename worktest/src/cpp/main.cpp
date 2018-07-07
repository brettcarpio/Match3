#include "Game.h"
#include <time.h>

//**********************************************************************
int main(int argc, char *argv[]) {
	srand(time(NULL));
	Game game;
	game.Start();
	return 0;
}


