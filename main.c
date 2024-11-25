#include <stdio.h>
#include "map.h"
#include "tree.h"
#include <limits.h>
#include "moves.h"


int main() {
    t_map map;

    // Test
    map = createMapFromFile("../maps/training.map");

    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);

    int start_x;
    int start_y;

    printf("Choose the start position (x,y) :\n");
    if (scanf("%d,%d", &start_x, &start_y) != 2) {
        printf("Invalid input format. Please use x,y.\n");
        return -1;
    }


    if (!isValidLocalisation((t_position){start_x, start_y}, map.x_max, map.y_max)) {
        printf("Invalid starting position (%d, %d). OVER\n", start_x, start_y);
        return -1;
    }

    // Example: Get the cost of the starting position
    int cost = getCost(map, (t_position){start_x, start_y});
    if (cost != COST_UNDEF) {
        printf("The cost at the starting position (%d, %d) is: %d\n\n", start_x, start_y, cost);
    } else {
        printf("Invalid cost at position (%d, %d). It might be out of bounds or uninitialized.\n\n", start_x, start_y);
    }


    t_tree* tree = initialize_tree_with_choices();


    t_localisation robot = loc_init(start_x, start_y, NORTH);
    //printf("Robot initialized at (%d, %d) facing %s\n\n", robot.pos.x, robot.pos.y, orientationToString(robot.ori));


    // Example movements
    /*
    robot.pos = UP(robot.pos);
    if (isValidLocalisation(robot.pos, map.x_max, map.y_max)) {
        printf("Robot moved UP to (%d, %d)\n", robot.pos.x, robot.pos.y);
    } else {
        printf("Robot tried to move UP out of bounds.\n");
    }

    robot.pos = RIGHT(robot.pos);
    if (isValidLocalisation(robot.pos, map.x_max, map.y_max)) {
        printf("Robot moved RIGHT to (%d, %d)\n", robot.pos.x, robot.pos.y);
    } else {
        printf("Robot tried to move RIGHT out of bounds.\n");
    }
    */

    displayMoveCosts(map, robot);

    // Find minimum cost leaf
    int min_cost = INT_MAX;
    t_node* min_leaf = NULL;
    min_leaf = find_minimum_cost_leaf(tree->tree->children[0], &min_cost, &min_leaf);

    if (min_leaf != NULL)
    {
        printf("Minimum cost leaf found with cost: %d\n", min_cost);
        printf("Path to minimum cost leaf:\n");
        trace_path_to_leaf(min_leaf);
    }
    else
    {
        printf("No leaf found in the tree.\n");
    }
    // Print the tree structure
    print_tree(tree->tree->children[0]);

    // Clean up the tree
    free_tree(tree);
    // Clean up the tree
    free_tree(tree);

    return 0;
}
