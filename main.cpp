#include "GameSDL.h"
#include "constParams.h"

Game* game = nullptr;

int main(int argc, char* argv[])
{
	game = new Game();

	game->init("DoomDay", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, img_w, img_h, false);

	while (game->running())
	{
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	return 0;
}
