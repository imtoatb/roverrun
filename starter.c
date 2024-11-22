//
// Created by idfcg on 22/11/2024.
//

#include <stdio.h>
#include "map.h"
#include "moves.h"


#define MAX_COMMANDS 100

// Initilaze the start position based on the given map
int initialize_start_position(t_map map, int* start_x, int* start_y) {
    printf("Choose the start position (column,row) :\n");
    if (scanf("%d,%d", start_x, start_y) != 2) {
        printf("Invalid input format. Please use column,row.\n");
        return -1;
    }

    if (!isValidLocalisation((t_position){*start_x, *start_y}, map.x_max, map.y_max)) {
        printf("Invalid starting position (%d, %d). OVER\n", *start_x, *start_y);
        return -1;
    }

    int cost = getCost(map, (t_position){*start_x, *start_y});
    if (cost > 10000) {
        printf("(%d, %d) is a crevasse, you're dead. Please choose another position.\n", *start_x, *start_y);
        return -1;
    }

    else {
        printf("The cost at the starting position (%d, %d) is: %d\n", *start_x, *start_y, cost);
    }

    return 0; // Success
}