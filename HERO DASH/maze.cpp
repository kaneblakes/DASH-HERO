#include "maze.h"
#include <fstream>
#include <iostream>
#include <sstream>

Maze::Maze() {}

Maze::~Maze() {}

void Maze::loadMaze(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open maze file: " << filePath << std::endl;
        return;
    }

    mazeData.clear();
    std::string line;

    // Parse metadata (START and SUCCESS positions)
    while (std::getline(file, line)) {
        if (line.rfind("START", 0) == 0) {
            std::istringstream stream(line);
            std::string keyword;
            int x, y;
            stream >> keyword >> x >> y;
            startPosition = { x, y };
        }
        else if (line.rfind("SUCCESS", 0) == 0) {
            std::istringstream stream(line);
            std::string keyword;
            int x, y;
            stream >> keyword >> x >> y;
            successPosition = { x, y };
        }
        else {
            // Parse maze layout
            std::vector<int> row;
            std::istringstream stream(line);
            int cell;
            while (stream >> cell) {
                row.push_back(cell);
            }
            if (!row.empty()) {
                mazeData.push_back(row);
            }
        }
    }

    file.close();
}

const std::vector<std::vector<int>>& Maze::getMazeData() const {
    return mazeData;
}

std::vector<std::vector<int>>& Maze::getMazeData() { // Non-const version
    return mazeData;
}

std::pair<int, int> Maze::getStartPosition() const {
    return startPosition;
}

std::pair<int, int> Maze::getSuccessPosition() const {
    return successPosition;
}
