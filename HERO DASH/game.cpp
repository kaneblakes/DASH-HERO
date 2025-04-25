#include "game.h"
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include <map>
#include <utility> // For std::pair
#include <SDL_mixer.h>

Game::Game() {}
Game::~Game() {}

Direction characterDirection = DOWN; // Default direction

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	this->velocityX = 0;
	this->velocityY = 0;
	this->tileHeight = 100; // Set tile height
	this->tileWidth = 100; // Set tile width

	IMG_Init(IMG_INIT_PNG);
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialized!..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			std::cout << "Renderer created!" << std::endl;

			backgroundMusic = Mix_LoadMUS("assets/sounds/bgm.wav");
			if (!backgroundMusic) {
				std::cerr << "Failed to load background music: " << Mix_GetError() << std::endl;
			}
			else {
				Mix_PlayMusic(backgroundMusic, -1); // Play music in a loop
			}

			// Initialize SDL_mixer
			// Load textures
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

			backgroundTexture = IMG_LoadTexture(renderer, "assets/bg/background.jpg");
			if (!backgroundTexture) {
				std::cout << "Failed to load background texture: " << SDL_GetError() << std::endl;
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

			heartTexture = IMG_LoadTexture(renderer, "assets/health.png");
			if (!heartTexture) {
				std::cout << "Failed to load health texture: " << SDL_GetError() << std::endl;
			}

			heartRect = { 0, 0, 32, 32 };

			// Query button texture dimensions
			int retryButtonWidth, retryButtonHeight;
			SDL_QueryTexture(retryButtonTexture, nullptr, nullptr, &retryButtonWidth, &retryButtonHeight);

			int continueButtonWidth, continueButtonHeight;
			SDL_QueryTexture(continueButtonTexture, nullptr, nullptr, &continueButtonWidth, &continueButtonHeight);

			// Calculate button positions
			int spacing = 20; // Space between buttons
			int totalWidth = retryButtonWidth + continueButtonWidth + spacing;
			int startX = (width - totalWidth) / 2; // Center horizontally
			int centerY = (height - retryButtonHeight) / 2; // Center vertically

			// Set button rectangles
			retryButtonRect = { startX, centerY, retryButtonWidth, retryButtonHeight };
			continueButtonRect = { startX + retryButtonWidth + spacing, centerY, continueButtonWidth, continueButtonHeight };

			// Initialize levels
			levelFiles = { "assets/level/maze1.txt", "assets/level/maze2.txt", "assets/level/maze3.txt", "assets/level/maze4.txt", "assets/level/maze5.txt","assets/level/maze6.txt", "assets/level/maze7.txt", "assets/level/maze8.txt", "assets/level/maze9.txt", "assets/level/maze10.txt","assets/level/maze11.txt", "assets/level/maze12.txt", "assets/level/maze13.txt", "assets/level/maze14.txt", "assets/level/maze15.txt" };
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

// Declare the variable once at the beginning of the function or scope
Mix_Chunk* changesound = nullptr;

void Game::handleEvents() {
	SDL_PollEvent(&event); // Poll for events
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (isEscapeScreenActive) {
			break;
		}
		switch (event.key.keysym.sym) {
		case SDLK_r:
			characterDirection = DOWN;
			loadLevel(currentLevel); // Reload the current level
			break;
		case SDLK_ESCAPE:
			isEscapeScreenActive = !isEscapeScreenActive; // Toggle escape screen
			break;
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_LEFT:
		case SDLK_RIGHT:
			// Load the sound only once
			if (!changesound) {
				changesound = Mix_LoadWAV("assets/sounds/move.wav");
				if (!changesound) {
					std::cerr << "Failed to load move sound: " << Mix_GetError() << std::endl;
				}
			}
			if (changesound) {
				Mix_PlayChannel(-1, changesound, 0);
			}

			// Update movement and direction
			health--;
			if (event.key.keysym.sym == SDLK_UP) {
				targetY = characterRect.y - tileHeight;
				targetX = characterRect.x;
				velocityX = 0;
				velocityY = -1;
				characterDirection = UP;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				targetY = characterRect.y + tileHeight;
				targetX = characterRect.x;
				velocityX = 0;
				velocityY = 1;
				characterDirection = DOWN;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				targetX = characterRect.x - tileWidth;
				targetY = characterRect.y;
				velocityX = -1;
				velocityY = 0;
				characterDirection = LEFT;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				targetX = characterRect.x + tileWidth;
				targetY = characterRect.y;
				velocityX = 1;
				velocityY = 0;
				characterDirection = RIGHT;
			}
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (isEscapeScreenActive) {
			int mouseX = event.button.x;
			int mouseY = event.button.y;

			// Check if the Retry button was clicked
			if (mouseX >= retryButtonRect.x && mouseX <= retryButtonRect.x + retryButtonRect.w &&
				mouseY >= retryButtonRect.y && mouseY <= retryButtonRect.y + retryButtonRect.h) {
				characterDirection = DOWN;
				loadLevel(currentLevel); // Reload the current level
				isEscapeScreenActive = false; // Close escape screen
			}

			// Check if the Continue button was clicked
			if (mouseX >= continueButtonRect.x && mouseX <= continueButtonRect.x + continueButtonRect.w &&
				mouseY >= continueButtonRect.y && mouseY <= continueButtonRect.y + continueButtonRect.h) {
				isEscapeScreenActive = false; // Close escape screen
			}
		}
		break;
	default:
		break;
	}
}



void Game::update() {
	// Skip updating the game state if the retry screen is active
	if (isEscapeScreenActive) {
		return;
	}

	auto& mazeData = maze.getMazeData(); // Get the maze data
	int rows = static_cast<int>(mazeData.size()); // Declare and initialize rows
	int cols = rows > 0 ? static_cast<int>(mazeData[0].size()) : 0; // Declare and initialize cols

	// Ensure maze data is valid
	if (mazeData.empty() || mazeData[0].empty()) {
		std::cerr << "Error: Maze data is empty or invalid for level " << currentLevel << std::endl;
		isRunning = false;
		return;
	}

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

	// Calculate the current and target tile indices
	int currentTileX = characterRect.x / tileWidth;
	int currentTileY = characterRect.y / tileHeight;
	int targetTileX = nextX / tileWidth;
	int targetTileY = nextY / tileHeight;

	// Check for success
	auto successPos = maze.getSuccessPosition();
	int successX = successPos.first * tileWidth;
	int successY = successPos.second * tileHeight;
	if (characterRect.x == successX && characterRect.y == successY) {
		// Play success sound
		Mix_Chunk* successSound = Mix_LoadWAV("assets/sounds/success.wav");
		if (successSound) {
			Mix_PlayChannel(-1, successSound, 0);
		}
		else {
			std::cerr << "Failed to load success sound: " << Mix_GetError() << std::endl;
		}

		std::cout << "Level " << currentLevel + 1 << " complete!" << std::endl;

		// Introduce a 2000ms buffer
		Uint32 startTime = SDL_GetTicks();
		while (SDL_GetTicks() - startTime < 1000) {
			SDL_Delay(1); // Delay for 1ms to avoid busy-waiting
		}

		currentLevel++;
		characterDirection = DOWN;
		if (currentLevel < levelFiles.size()) {
			loadLevel(currentLevel); // Load the next level
		}
		else {
			std::cout << "All levels complete! You win!" << std::endl;
			isRunning = false; // Stop the game
		}
		return; // Skip further processing
	}

	// Check if the target tile is within bounds
	if (targetTileY >= 0 && targetTileY < rows && targetTileX >= 0 && targetTileX < cols) {
		if (mazeData[targetTileY][targetTileX] == 0 || mazeData[targetTileY][targetTileX] == 2 || mazeData[targetTileY][targetTileX] == 3) {
			// Allow movement onto open space (0), wall2 (2), or wall3 (3)
			characterRect.x = nextX;
			characterRect.y = nextY;

			// Transition wall3 to wall2 after moving past it
			if (mazeData[currentTileY][currentTileX] == 3) {
				mazeData[currentTileY][currentTileX] = 2; // Change logic to wall2
				Mix_Chunk* changesound = Mix_LoadWAV("assets/sounds/change.wav");
				if (changesound) {
					Mix_PlayChannel(-1, changesound, 0);
				}
				else {
					std::cerr << "Failed to load change sound: " << Mix_GetError() << std::endl;
				}
			}
			else if (mazeData[currentTileY][currentTileX] == 2) {
				// Transition wall2 to wall1 after moving past it
				mazeData[currentTileY][currentTileX] = 1; // Change logic to wall1
				Mix_Chunk* changesound = Mix_LoadWAV("assets/sounds/change.wav");
				if (changesound) {
					Mix_PlayChannel(-1, changesound, 0);
				}
				else {
					std::cerr << "Failed to load change sound: " << Mix_GetError() << std::endl;
				}
			}
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

	if (health <= 0 && !(characterRect.x == successX && characterRect.y == successY)) {
		isEscapeScreenActive = true;
	}
}

void Game::render() {
	SDL_RenderClear(renderer); // Clear the screen

	// Render the background
	if (backgroundTexture) {
		SDL_Rect backgroundRect = { 0, 0, 800, 600 }; // Adjust to match your window size
		SDL_RenderCopy(renderer, backgroundTexture, nullptr, &backgroundRect);
	}

	if (isEscapeScreenActive) {
		// Render the game screen first
		const auto& mazeData = maze.getMazeData();
		for (size_t i = 0; i < mazeData.size(); ++i) {
			for (size_t j = 0; j < mazeData[i].size(); ++j) {
				if (mazeData[i][j] == 1) { // Wall
					SDL_Rect wallRect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
					SDL_RenderCopy(renderer, wallTexture, nullptr, &wallRect);
				}
				else if (mazeData[i][j] == 2) { // Wall type 2
					SDL_Rect wall2Rect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
					SDL_RenderCopy(renderer, wall2Texture, nullptr, &wall2Rect);
				}
				else if (mazeData[i][j] == 3) { // Wall type 3
					SDL_Rect wall3Rect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
					SDL_RenderCopy(renderer, wall3Texture, nullptr, &wall3Rect);
				}
			}
		}
		SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 25); // Black with 10% opacity (alpha = 25 out of 255)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Enable blending for transparency
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
			else if (mazeData[i][j] == 2) { // Wall type 2
				SDL_Rect wall2Rect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
				SDL_RenderCopy(renderer, wall2Texture, nullptr, &wall2Rect);
			}
			else if (mazeData[i][j] == 3) { // Wall type 3
				SDL_Rect wall3Rect = { static_cast<int>(j * tileWidth), static_cast<int>(i * tileHeight), tileWidth, tileHeight };
				SDL_RenderCopy(renderer, wall3Texture, nullptr, &wall3Rect);
			}
		}
	}

	SDL_Texture* currentCharacterTexture = nullptr;
	switch (characterDirection) {
	case UP:
		currentCharacterTexture = IMG_LoadTexture(renderer, "assets/player/hero_up.png");
		break;
	case DOWN:
		currentCharacterTexture = IMG_LoadTexture(renderer, "assets/player/hero_down.png");
		break;
	case LEFT:
		currentCharacterTexture = IMG_LoadTexture(renderer, "assets/player/hero_left.png");
		break;
	case RIGHT:
		currentCharacterTexture = IMG_LoadTexture(renderer, "assets/player/hero_right.png");
		break;
	}

	if (currentCharacterTexture) {
		SDL_RenderCopy(renderer, currentCharacterTexture, nullptr, &characterRect);
		SDL_DestroyTexture(currentCharacterTexture); // Free the texture after use
	}

	// Render the health bar
	for (int i = 0; i < health; ++i) {
		SDL_Rect heartPosition = { 10 + i * (heartRect.w + 5), 10, heartRect.w, heartRect.h }; // Position hearts with spacing
		SDL_RenderCopy(renderer, heartTexture, nullptr, &heartPosition);
	}

	SDL_RenderCopy(renderer, characterTexture, nullptr, &characterRect);

	SDL_RenderPresent(renderer); // Present the renderer
}




void Game::loadLevel(int levelIndex) {
	if (levelIndex >= 0 && levelIndex < levelFiles.size()) {
		maze.loadMaze(levelFiles[levelIndex]); // Load the maze file

		// Parse the "MOVES" value from the maze file
		std::ifstream levelFile(levelFiles[levelIndex]);
		if (levelFile.is_open()) {
			std::string line;
			while (std::getline(levelFile, line)) {
				if (line.rfind("MOVES", 0) == 0) { // Check if the line starts with "MOVES"
					health = std::stoi(line.substr(6)); // Extract the number after "MOVES "
					break;
				}
			}
			levelFile.close();
		}
		else {
			std::cout << "Failed to open level file: " << levelFiles[levelIndex] << std::endl;
		}

		if (levelIndex >= 0 && levelIndex < levelFiles.size()) {
			maze.loadMaze(levelFiles[levelIndex]);

			// Reset character movement
			velocityX = 0;
			velocityY = 0;
			targetX = 0;
			targetY = 0;

			// Set the character's starting position
			auto startPos = maze.getStartPosition();
			characterRect.x = startPos.first * tileWidth;
			characterRect.y = startPos.second * tileHeight;
			characterRect.w = 100;
			characterRect.h = 100;
		}
	}
	else {
		std::cout << "Invalid level index: " << levelIndex << std::endl;
		isRunning = false; // Stop the game if no more levels
	}
}



void Game::cleanup() {
	SDL_DestroyTexture(heartTexture);
	Mix_FreeMusic(backgroundMusic);
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
	Mix_Quit();
	SDL_Quit();
	std::cout << "Game cleaned up!" << std::endl;
}

