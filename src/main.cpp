#define _CRTDBG_MAP_ALLOC
#include "../headers/GameSDL.h"
#include "../headers/constParams.h"
#include <cstdlib>
#include <crtdbg.h>

Game* game = nullptr;

int main(int argc, char* argv[])
{
	game = new Game();

	game->init("DoomDay", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, img_w, img_h, false);

	while (game->running())
	{
		lastTick = SDL_GetTicks();
		game->handleEvents();
		game->update();
		game->render();
		currTick = SDL_GetTicks();

		//frames delay
		deltaTime = currTick - lastTick;
		double mSec = 1000 / fpsLimit;
		if (mSec > deltaTime)
		{
			SDL_Delay(mSec - deltaTime);
			deltaTime = mSec;
		}
	}

	game->clean();
	delete game;

	return 0;
}