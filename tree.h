//
// Created by makethpanda on 13/11/2024.
//

#ifndef TREE_H
#define TREE_H

#include "loc.h"
#include "moves.h"
#include <stddef.h> // For NULL

// Node structure representing a position in the tree
typedef struct s_node {

    struct s_node *parent;     // Pointer to the parent node
    struct s_node **children; // Array of child node pointers
    t_position loc;           // Position associated with the node
    int cost;                 // Cost value for the node
    int possibilities;        // Number of child nodes
} t_node;

// Root structure containing the children of the root node and its cost
typedef struct s_root {
    struct s_node **children; // Array of root's child node pointers
    int cost;                 // Root node cost
} t_root;



typedef struct s_tree {
    t_root *tree; // Pointer to the root of the tree
} t_tree;


// Function prototypes

t_node* create_node(t_position loc, t_move move, int cost);
void free_node(t_node* node);
void add_child(t_node* parent, t_node* child);
t_tree* create_tree(t_position loc, t_move move, int cost);

void free_tree(t_tree* tree);
t_tree* initialize_tree_with_choices();
t_node* find_minimum_cost_leaf(t_node* node, int* min_cost, t_node** min_leaf);
void trace_path_to_leaf(t_node* leaf);
void print_tree(t_node* root);


#endif // TREE_H
