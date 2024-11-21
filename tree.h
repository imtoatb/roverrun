//
// Created by makethpanda on 13/11/2024.
//

#include "loc.h"
#include "moves.h"

#ifndef TREE_H
#define TREE_H

typedef struct s_node {
    struct s_node *parent;
    struct s_node **children;
    t_position loc;
    int cost;
    int possibilities;
} t_node;

typedef struct s_root {
    struct s_node **children;
    int cost;
} t_root;

typedef struct s_tree {
    t_root *tree;
} t_tree;


t_node* create_node(t_position loc, t_move move, int cost);
void free_node(t_node* node);
void add_child(t_node* parent, t_node* child);
t_tree* create_tree(t_position loc, t_move move, int cost);

void free_tree(t_tree* tree);

t_tree* initialize_tree_with_choices();
t_node* find_minimum_cost_leaf(t_node* node, int* min_cost, t_node** min_leaf);
void trace_path_to_leaf(t_node* leaf);

#endif //TREE_H