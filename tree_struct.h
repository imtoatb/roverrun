//
// Created by idfcg on 14/11/2024.
//

#include "loc.h"
#include "moves.h"

#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H


typedef struct s_node {
    struct s_node **children;
    struct s_node *parent;
    t_position localisation;
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

t_node* create_node(t_position localisation, t_move move, int cost);

void free_node(t_node* node);

void add_child(t_node* parent, t_node* child);

#endif //TREE_STRUCT_H
