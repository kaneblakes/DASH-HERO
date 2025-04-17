#include "game.h"
#include <SDL_image.h>

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	this->velocityX = 0;
	this->velocityY = 0;
	this->tileHeight = 100; // Set tile height
	this->tileWidth = 100; // Set tile width
	// Set button dimensions
	int buttonWidth = 200;
	int buttonHeight = 100;

	// Set button positions and sizes
	retryButtonRect = { (windowWidth - buttonWidth) / 2, (windowHeight - buttonHeight) / 2 - buttonHeight, buttonWidth, buttonHeight }; // Retry button
	continueButtonRect = { (windowWidth - buttonWidth) / 2, (windowHeight - buttonHeight) / 2 + 20, buttonWidth, buttonHeight }; // Continue button

	IMG_Init(IMG_INIT_PNG);
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialised!..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;

			wallTexture = IMG_LoadTexture(renderer, "assets/walls/wall.png");
			if (!wallTexture) {
				std::cout << "Failed to load wall texture: " << SDL_GetError() << std::endl;
			}

			wall2Texture = IMG_LoadTexture(renderer, "assets/walls/wall2.png");
			if (!wall2Texture) {
				std::cout << "Failed to load wall2 texture: " << SDL_GetError() << std::endl;
			}

			wall3Texture = IMG_LoadTexture(renderer, "assets/walls/wall3.png");
			if (!wall3Texture) {
				std::cout << "Failed to load wall3 texture: " << SDL_GetError() << std::endl;
			}

			characterTexture = IMG_LoadTexture(renderer, "assets/player/hero.png");
			if (!characterTexture) {
				std::cout << "Failed to load character texture: " << SDL_GetError() << std::endl;
			}

			retryButtonTexture = IMG_LoadTexture(renderer, "assets/buttons/retry.png");
			if (!retryButtonTexture) {
				std::cout << "Failed to load retry button texture: " << SDL_GetError() << std::endl;
			}

			continueButtonTexture = IMG_LoadTexture(renderer, "assets/buttons/continue.png");
			if (!continueButtonTexture) {
				std::cout << "Failed to load continue button texture: " << SDL_GetError() << std::endl;
			}

			// Set button positions and sizes
			retryButtonRect = { 200, 300, 200, 100 };    // Example position and size
			continueButtonRect = { 500, 300, 200, 100 }; // Example position and size

			// Initialize levels
			levelFiles = { "assets/level/maze1.txt", "assets/level/maze2.txt", "assets/level/maze3.txt", "assets/level/maze4.txt", "assets/level/maze5.txt", "assets/level/maze6.txt", "assets/level/maze7.txt", "assets/level/maze8.txt", "assets/level/maze9.txt", "assets/level/maze10.txt", "assets/level/maze11.txt", "assets/level/maze12.txt", "assets/level/maze13.txt", "assets/level/maze14.txt", "assets/level/maze15.txt" };
			currentLevel = 0;

			// Load the first level
			loadLevel(currentLevel);
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}
	isEscapeScreenActive = false;

}


void Game::handleEvents() {
	SDL_PollEvent(&event); // Poll for events
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			if (isEscapeScreenActive) {
				isEscapeScreenActive = false; // Close escape screen
			}
			else {
				isEscapeScreenActive = true; // Open escape screen
			}
			break;
		case SDLK_UP:
			targetY = characterRect.y - tileHeight; // Move up one tile
			targetX = characterRect.x;             // Keep the same X position
			velocityX = 0;
			velocityY = -1; // Move up
			break;
		case SDLK_DOWN:
			targetY = characterRect.y + tileHeight; // Move down one tile
			targetX = characterRect.x;              // Keep the same X position
			velocityX = 0;
			velocityY = 1; // Move down
			break;
		case SDLK_LEFT:
			targetX = characterRect.x - tileWidth; // Move left one tile
			targetY = characterRect.y;            // Keep the same Y position
			velocityX = -1; // Move left
			velocityY = 0;
			break;
		case SDLK_RIGHT:
			targetX = characterRect.x + tileWidth; // Move right one tile
			targetY = characterRect.y;            // Keep the same Y position
			velocityX = 1; // Move right
			velocityY = 0;
			break;
		}
		break;
	default:
		break;
	}
}



void Game::update() {
	auto& mazeData = maze.getMazeData();
	int rows = static_cast<int>(mazeData.size());
	int cols = rows > 0 ? static_cast<int>(mazeData[0].size()) : 0;

	// Determine the next position based on the current direction
	int nextX = characterRect.x;
	int nextY = characterRect.y;

	if (velocityX > 0) { // Moving right
		nextX += tileWidth;
	}
	else if (velocityX < 0) { // Moving left
		nextX -= tileWidth;
	}
	else if (velocityY > 0) { // Moving down
		nextY += tileHeight;
	}
	else if (velocityY < 0) { // Moving up
		nextY -= tileHeight;
	}

	// Calculate the target tile indices
	int targetTileX = nextX / tileWidth;
	int targetTileY = nextY / tileHeight;

	// Check if the target tile is within bounds and not a wall
	if (targetTileY >= 0 && targetTileY < rows && targetTileX >= 0 && targetTileX < cols) {
		if (mazeData[targetTileY][targetTileX] == 0) { // 0 represents open space
			characterRect.x = nextX;
			characterRect.y = nextY;
		}
		else {
			velocityX = 0;
			velocityY = 0; // Stop movement on collision
		}
	}
	else {
		velocityX = 0;
		velocityY = 0; // Stop movement out of bounds
	}

	if (mazeData[targetTileY][targetTileX] == 0 || mazeData[targetTileY][targetTileX] == 2 || mazeData[targetTileY][targetTileX] == 3) {
		characterRect.x = nextX;
		characterRect.y = nextY;

		// Handle tiles that require multiple passes
		if (mazeData[targetTileY][targetTileX] == 3) {
			mazeData[targetTileY][targetTileX] = 2; // Decrement to 2 on the first pass
		}
		else if (mazeData[targetTileY][targetTileX] == 2) {
			mazeData[targetTileY][targetTileX] = 1; // Convert to 1 on the second pass
		}
	}

	// Check for success
	auto successPos = maze.getSuccessPosition();
	int successX = successPos.first * tileWidth;
	int successY = successPos.second * tileHeight;
	if (characterRect.x == successX && characterRect.y == successY) {
		std::cout << "Level " << currentLevel + 1 << " complete!" << std::endl;
		currentLevel++;
		if (currentLevel < levelFiles.size()) {
			loadLevel(currentLevel); // Load the next level
		}
		else {
			std::cout << "All levels complete! You win!" << std::endl;
			isRunning = false; // Stop the game
		}
	}
}





void Game::render() {
	SDL_RenderClear(renderer); // Clear the screen

	if (isEscapeScreenActive) {
		// Render the game screen first
		const auto& mazeData = maze.getMazeData();
		for (size_t i = 0; i < mazeData.size(); ++i) {
			for (size_t j = 0; j < mazeData[i].size(); ++j) {
				if (mazeData[i][j] == 1) { // Wall
					SDL_Rect wallRect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
					SDL_RenderCopy(renderer, wallTexture, nullptr, &wallRect);
				}
			}
		}
		SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);

		// Render a semi-transparent overlay
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 25); // Black with 10% opacity
		SDL_Rect overlayRect = { 0, 0, 800, 600 }; // Adjust to match your window size
		SDL_RenderFillRect(renderer, &overlayRect);

		// Render the Retry button
		if (retryButtonTexture) {
			SDL_RenderCopy(renderer, retryButtonTexture, nullptr, &retryButtonRect);
		}

		// Render the Continue button
		if (continueButtonTexture) {
			SDL_RenderCopy(renderer, continueButtonTexture, nullptr, &continueButtonRect);
		}

		SDL_RenderPresent(renderer);
		return; // Skip rendering the game
	}

	// Render the game as usual
	const auto& mazeData = maze.getMazeData();
	for (size_t i = 0; i < mazeData.size(); ++i) {
		for (size_t j = 0; j < mazeData[i].size(); ++j) {
			if (mazeData[i][j] == 1) { // Wall
				SDL_Rect wallRect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
				SDL_RenderCopy(renderer, wallTexture, nullptr, &wallRect);
			}
		}
	}
	SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);

	SDL_RenderPresent(renderer); // Present the renderer
}


void Game::loadLevel(int levelIndex) {
	if (levelIndex >= 0 && levelIndex < levelFiles.size()) {
		maze.loadMaze(levelFiles[levelIndex]); // Load the maze file
		auto startPos = maze.getStartPosition();
		characterRect.x = startPos.first * tileWidth; // Set X position
		characterRect.y = startPos.second * tileHeight; // Set Y position
		characterRect.w = 100; // Set character width
		characterRect.h = 100; // Set character height
	}
	else {
		std::cout << "Invalid level index: " << levelIndex << std::endl;
		isRunning = false; // Stop the game if no more levels
	}
}





void Game::cleanup() {
	SDL_DestroyTexture(retryButtonTexture);
	SDL_DestroyTexture(continueButtonTexture);
	SDL_DestroyTexture(characterTexture); // Destroy the hero texture
	SDL_DestroyTexture(wallTexture); // Destroy the wall texture
	SDL_DestroyTexture(wall2Texture); // Destroy the wall2 texture
	SDL_DestroyTexture(wall3Texture); // Destroy the wall3 texture
	SDL_DestroyTexture(backgroundTexture); // Destroy the background texture
	SDL_DestroyTexture(successTexture); // Destroy the success texture
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Game cleaned up!" << std::endl;
}

