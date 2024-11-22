//
// Created by flasque on 22/11/2024.
//

#ifndef UNTITLED1_ALGO_H
#define UNTITLED1_ALGO_H

#include "map.h"
#include "tree.h"

/**
 * @brief Finds the 3 shortest paths from the base station to a given location.
 * @param map: The map containing the terrain and cost information.
 * @param destination: The destination position to which paths are calculated.
 * @return A tree containing the 3 shortest paths as separate branches.
 */
t_tree* find_three_shortest_paths(t_map map, t_position destination);

/**
 * @brief Displays the 3 shortest paths in a human-readable format.
 * @param tree: The tree containing the 3 shortest paths.
 */
void display_three_shortest_paths(t_tree* tree);

#endif // UNTITLED1_ALGO_H
