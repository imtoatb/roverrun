#include <stdio.h>
#include "pathfinding.h"
#include <math.h>
#include "map.h"
#include "tree.h"
#include "loc.h"



// Define and initialize the configuration
Config config = {7, 6, 12, 3, 20};

// Sample map for testing
int maptest[7][6] = {
    {7, 3, 2, 2, 10002, 13},
    {3, 1, 0, 1, 5, 9},
    {3, 2, 1, 5, 6, 7},
    {5, 3, 10001, 9, 7, 8},
    {7, 5, 9, 13, 8, 9},
    {8, 6, 7, 14, 9, 10},
    {10, 8, 9, 10014, 10, 11}
};

int main() {

    t_map mapwork;

    mapwork = createMapFromFile("maps\\example1.map");


    printf("Map created with dimensions %d x %d\n", mapwork.y_max, mapwork.x_max);
    for (int i = 0; i < mapwork.y_max; i++)
    {
        for (int j = 0; j < mapwork.x_max; j++)
        {
            printf("%d ", mapwork.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < mapwork.y_max; i++)
    {
        for (int j = 0; j < mapwork.x_max; j++)
        {
            printf("%-5d ", mapwork.costs[i][j]);
        }
        printf("\n");
    }
    // Now define the map array for costs
    int map[7][6] = {0}; // Make sure to initialize the map array

    // Copy costs from mapwork into the map array
    for (int i = 0; i < mapwork.y_max; i++) {
        for (int j = 0; j < mapwork.x_max; j++) {
            map[i][j] = mapwork.costs[i][j];
        }
    }
    displayMap(mapwork);
    //the map has been displayed
    int cols = mapwork.y_max;
    int rows = mapwork.x_max;
    config.cols = cols;
    config.rows = rows;
    config.max_cost = mapwork.x_max * mapwork.y_max;
    printf("Max cost is %d\n", config.max_cost);

    int startX = 0, startY = 0;  // Starting position
    int targetTile = 0;          // Target tile value (e.g., find "1" on the map)

    // Clear the allPaths array
    for (int i = 0; i < config.max_paths; i++) {
        allPaths[i].length = 0;
        allPaths[i].cost = config.max_cost; // Initialize to max cost
    }

    // Initialize a Path object for tracking the current path
    Path currentPath;
    currentPath.length = 0;
    currentPath.cost = 0;

    // Perform DFS to find paths
    printf("Finding paths from (%d, %d) to target tile %d...\n\n", startX, startY, targetTile);
    dfs(startX, startY, 0, map, targetTile, currentPath, 1);

    // Print the results
    printf("Top %d shortest paths found:\n", config.max_paths);
    for (int i = 0; i < config.max_paths; i++) {
        if (allPaths[i].length > 0) {
            printf("Path %d: ", i + 1);
            printPath(allPaths[i]);
        } else {
            printf("Path %d: No valid path found.\n", i + 1);
        }
    }

    return 0;
}
