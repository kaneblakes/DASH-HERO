#pragma once
#include "maze.h"
#include <SDL.h>
#include <iostream>
#include <vector>
#include <map>

class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void run();
    void loadLevel(int levelIndex);
    void cleanup();
    void handleEvents();
    void update();
    void render();
    bool running() { return isRunning; }

private:
    int count;
    std::map<std::pair<int, int>, Uint32> tileTimers; // Map to store tile timers
    bool showRetryMenuOnNextInput; // Flag to show retry menu on the next input
    int targetTileX; // X-coordinate of the target tile
    int targetTileY; // Y-coordinate of the target tile
    SDL_Texture* retryButtonTexture;    // Texture for the Retry button
    SDL_Texture* continueButtonTexture; // Texture for the Continue button
    SDL_Rect retryButtonRect;           // Rectangle for the Retry button
    SDL_Rect continueButtonRect;        // Rectangle for the Continue button
    int health; // Number of remaining moves (hearts)
    SDL_Texture* heartTexture; // Texture for the heart image
    SDL_Rect heartRect; // Rectangle for a single heart
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    int score;
    int lives;
    int level;
    int rows; // Number of rows in the maze
    int cols; // Number of columns in the maze
    std::vector<std::string> levelData;
    SDL_Texture* characterTexture;
    SDL_Texture* wallTexture;  // Texture for wall type 1
    SDL_Texture* wall2Texture; // Texture for wall type 2
    SDL_Texture* wall3Texture; // Texture for wall type 3
    bool isEscapeScreenActive; // Tracks if the escape screen is active
    bool isWithinBounds(int y, int x) const {
        return y >= 0 && y < rows && x >= 0 && x < cols;
    }
    SDL_Texture* backgroundTexture; // Texture for the background
    SDL_Texture* successTexture; // Texture for the success screen
    SDL_Rect characterRect;
    int velocityX; // Horizontal velocity
    int velocityY; // Vertical velocity
    int targetX; // Target X position for grid movement
    int targetY; // Target Y position for grid movement
    int tileWidth;  // Width of a single tile
    int tileHeight; // Height of a single tile
    int currentLevel; // Tracks the current level
    std::vector<std::string> levelFiles; // List of maze file paths
    Maze maze;
};
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

extern Direction characterDirection; // Declare the variable as external


