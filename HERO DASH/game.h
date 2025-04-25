#pragma once
#include "maze.h"
#include <SDL.h>
#include <SDL_mixer.h> 
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
    Mix_Music* backgroundMusic; // Now recognized as a valid type
    std::map<std::pair<int, int>, Uint32> tileTimers;
    bool showRetryMenuOnNextInput;
    int targetTileX;
    int targetTileY;
    SDL_Texture* retryButtonTexture;
    SDL_Texture* continueButtonTexture;
    SDL_Rect retryButtonRect;
    SDL_Rect continueButtonRect;
    int health;
    SDL_Texture* heartTexture;
    SDL_Rect heartRect;
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    int score;
    int lives;
    int level;
    int rows;
    int cols;
    std::vector<std::string> levelData;
    SDL_Texture* characterTexture;
    SDL_Texture* wallTexture;
    SDL_Texture* wall2Texture;
    SDL_Texture* wall3Texture;
    bool isEscapeScreenActive;
    bool isWithinBounds(int y, int x) const {
        return y >= 0 && y < rows && x >= 0 && x < cols;
    }
    SDL_Texture* backgroundTexture;
    SDL_Texture* successTexture;
    SDL_Rect characterRect;
    int velocityX;
    int velocityY;
    int targetX;
    int targetY;
    int tileWidth;
    int tileHeight;
    int currentLevel;
    std::vector<std::string> levelFiles;
    Maze maze;
};

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

extern Direction characterDirection; // Declare the variable as external