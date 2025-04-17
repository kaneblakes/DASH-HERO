#pragma once
#include <vector>
#include <string>

class Maze {
public:
    Maze();
    ~Maze();
    void loadMaze(const std::string& filePath);
    std::pair<int, int> getStartPosition() const;
    std::pair<int, int> getSuccessPosition() const;
    const std::vector<std::vector<int>>& getMazeData() const;
    std::vector<std::vector<int>>& getMazeData(); // Non-const version
    int getMoveLimit() const; // Getter for move limit

private:
    std::vector<std::vector<int>> mazeData; // 2D grid representing the maze
    std::pair<int, int> startPosition;      // Starting position (x, y)
    std::pair<int, int> successPosition;    // Success position (x, y)
    int moveLimit;                          // Maximum moves allowed
};
