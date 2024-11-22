#include <stdio.h>
#include "map.h"
#include "tree.h"
#include "loc.h"
#include "starter.h"

#include <limits.h>

int main() {
    t_map map;
    map = createMapFromFile("../maps/example1.map");

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

    //--------------------------------------------------------------------------------------------------------------------------------//

    int start_x;
    int start_y;

    //In starter.c
    if (initialize_start_position(map, &start_x, &start_y) == -1) {
        return -1;
    }


    t_tree* tree = initialize_tree_with_choices();


    t_localisation robot = loc_init(start_x, start_y, NORTH);
    printf("Robot initialized at (%d, %d) facing %s\n\n", robot.pos.x, robot.pos.y, orientationToString(robot.ori));




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

    // Clean up the tree
    free_tree(tree);

    //testMovements(&robot, map);

    return 0;
}
