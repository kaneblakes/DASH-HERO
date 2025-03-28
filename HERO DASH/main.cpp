#include <SDL.h>
#include <iostream>
#include <cmath>
#include "Levels.h"  // Include the header so we can use 'levels'

int main(int argc, char* argv[]) {
    // For demonstration, just pick a level index
    int currentLevel = 1;

    // Retrieve the maze layout and starting position
    auto maze = levels[currentLevel].maze;
    int startX = levels[currentLevel].startX;
    int startY = levels[currentLevel].startY;

    // Print some info
    std::cout << "Loaded level " << (currentLevel + 1)
        << " with a maze of size " << maze.size()
        << " rows x " << maze[0].size() << " cols.\n";
    std::cout << "Starting position: (" << startX << ", " << startY << ")\n";

    // --------------------
    // 1) Initialize SDL
    // 2) Create Window/Renderer
    // 3) Use 'maze' for your game logic
    // 4) (Optional) Slide or move the player starting at (startX, startY)
    // 5) Main loop, rendering, etc.
    // --------------------

    // Minimal example just for demonstration:
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL Maze",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // ... Your rendering loop, etc. ...

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
