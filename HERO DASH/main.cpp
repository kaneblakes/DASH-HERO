#define SDL_MAIN_HANDLED
#include "game.h"

Game *game = nullptr;

int main(int argc, const char* argv[]) {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	game = new Game();

	game->init("HERO DASH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->running()) {
		frameStart = SDL_GetTicks();
		game->handleEvents();
		game->update();
		game->render();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}		
	}

	game->cleanup();

	return 0;
}