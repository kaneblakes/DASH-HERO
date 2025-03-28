#include "Levels.h"

// Define the actual data for our levels
const std::vector<Level> levels = {
    // Level 1
    {
        {
            {1,1,1,1,1},
            {1,0,0,0,1},
            {1,0,1,0,1},
            {1,0,0,2,1},
            {1,1,1,1,1}
        },
        1, // startX
        1  // startY
    },

    // Level 2 (example of different layout + start)
    {
        {
            {1,1,1,1,1,1},
            {1,0,0,0,1,1},
            {1,0,1,0,0,1},
            {1,0,0,2,0,1},
            {1,1,1,1,1,1}
        },
        2, // startX
        1  // startY
    }

    // You can add more levels here
};
