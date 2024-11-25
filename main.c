#include <stdio.h>
#include <math.h>

#include "pathfinding.h"
#include "map.h"
#include "tree.h"
#include "loc.h"

#define MAX_MAPS 10

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

//Global function
t_map mapwork;
int num_maps;
int choice;
int initialize_start_position(t_map mapwork, int * start_x, int * start_y);

int main() {

    //mapwork = createMapFromFile("../maps\\example1.map");

    const char *map_files[MAX_MAPS] = {                             //To enable the user to choose the map
        "../maps/example1.map",
        "../maps/example2.map",
        "../maps/example3.map",
        "../maps/example4.map"
    };
    num_maps = 4;                                                   //Update this if more maps are added


    //Menu + Input
    printf("Choose a map to load:\n");
    for (int i = 0; i < num_maps; i++) {
        printf("[%d] %s\n", i + 1, map_files[i]);
    }
    printf("Enter the number of the map you want to use: ");


    if (scanf("%d", &choice) != 1 || choice < 1 || choice > num_maps) {
        printf("Invalid choice. Exiting...\n");
        return -1;
    }


    //Load the map
    const char *selected_map = map_files[choice - 1];               //Declare and initialize `selected_map`
    printf("Loading map: %s\n", selected_map);

    mapwork = createMapFromFile(selected_map);
    if (mapwork.soils == NULL) {
        printf("Failed to load map. Exiting...\n");
        return -1;
    }




    printf("Map created with dimensions %d x %d\n", mapwork.y_max, mapwork.x_max);
    for (int i = 0; i < mapwork.y_max; i++)
    {
        for (int j = 0; j < mapwork.x_max; j++)
        {
            printf("%d ", mapwork.soils[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < mapwork.y_max; i++)
    {
        for (int j = 0; j < mapwork.x_max; j++)
        {
            printf("%-5d ", mapwork.costs[i][j]);                  //Printf the costs, aligned left 5 digits
        }
        printf("\n");
    }


    int map[7][6] = {0};                                                //Make sure to initialize the map array

    // Copy costs from mapwork into the map array
    for (int i = 0; i < mapwork.y_max; i++) {
        for (int j = 0; j < mapwork.x_max; j++) {
            map[i][j] = mapwork.costs[i][j];
        }
    }
    displayMap(mapwork);


    int cols = mapwork.y_max;
    int rows = mapwork.x_max;
    config.cols = cols;
    config.rows = rows;
    config.max_cost = mapwork.x_max * mapwork.y_max;
    printf("Max cost is %d\n", config.max_cost);


    // Starting position
    int start_x;
    int start_y;

    //In starter.c
    if (initialize_start_position(mapwork, &start_x, &start_y) == -1) {
        return -1;
    }


    t_localisation robot = loc_init(start_x, start_y, NORTH);
    printf("Robot initialized at (%d, %d) facing %s\n\n", robot.pos.x, robot.pos.y, orientationToString(robot.ori));


    int targetTile = 0;                                             // Target tile value (e.g., find "1" on the map)

    // Clear the allPaths array
    for (int i = 0; i < config.max_paths; i++) {
        allPaths[i].length = 0;
        allPaths[i].cost = config.max_cost;                         // Initialize to max cost
    }

    // Initialize a Path object for tracking the current path
    Path currentPath;
    currentPath.length = 0;
    currentPath.cost = 0;

    // Perform DFS to find paths
    printf("Finding paths from (%d, %d) to target tile %d...\n\n", start_x, start_y, targetTile);
    dfs(start_x, start_y, 0, map, targetTile, currentPath, 1);


    printf("\n");
    checkValidMove(mapwork, robot);
    printf("\n\n");



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
