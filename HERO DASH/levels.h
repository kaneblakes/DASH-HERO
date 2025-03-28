#pragma once
#include <vector>

// A struct to hold one level's data
struct Level {
    // The maze layout: 1 = wall, 0 = path, 2 = exit
    std::vector<std::vector<int>> maze;
    // Starting position (grid coordinates)
    int startX;
    int startY;
};

// Declare a global "levels" array (vector) that we'll define in Levels.cpp
extern const std::vector<Level> levels;
