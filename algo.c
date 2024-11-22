#include <stdio.h>
#include <stdlib.h>
#include "algo.h"
#include "queue.h"

t_tree* find_three_shortest_paths(t_map map, t_position target)
{
    t_position base = getBaseStationPosition(map);
    t_tree* tree = create_tree(base, F_10, 0);

    t_queue queue = createQueue(map.x_max * map.y_max);
    enqueue(&queue, base);

    while (queue.first != queue.last)
    {
        t_position current = dequeue(&queue);
        int current_cost = map.costs[current.y][current.x];

        // If we've reached the target, stop processing further
        if (current.x == target.x && current.y == target.y)
            break;

        // Check neighbors
        t_position neighbors[] = {LEFT(current), RIGHT(current), UP(current), DOWN(current)};
        for (int i = 0; i < 4; i++)
        {
            t_position neighbor = neighbors[i];
            if (isValidLocalisation(neighbor, map.x_max, map.y_max))
            {
                int new_cost = current_cost + _soil_cost[map.soils[neighbor.y][neighbor.x]];
                if (new_cost < map.costs[neighbor.y][neighbor.x])
                {
                    map.costs[neighbor.y][neighbor.x] = new_cost;
                    enqueue(&queue, neighbor);

                    // Add the neighbor to the tree
                    t_node* parent = find_node_in_tree(tree->tree->children[0], current);
                    t_node* child = create_node(neighbor, F_10, new_cost);
                    add_child(parent, child);
                }
            }
        }
    }

    // Extract the three shortest paths to the target
    int min_costs[3] = {COST_UNDEF, COST_UNDEF, COST_UNDEF};
    t_node* min_leaves[3] = {NULL, NULL, NULL};

    find_k_minimum_cost_leaves(tree->tree->children[0], min_costs, min_leaves, 3);

    printf("Three shortest paths:\n");
    for (int i = 0; i < 3 && min_leaves[i]; i++)
    {
        printf("Path %d:\n", i + 1);
        trace_path_to_leaf(min_leaves[i]);
    }

    return tree;
}
