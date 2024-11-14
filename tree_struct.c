//
// Created by idfcg on 14/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "tree_struct.h"
#include "moves.h"


t_node* create_node(t_position localisation, t_move move, int cost) {

    t_node* node = (t_node*)malloc(sizeof(t_node));
    if (!node) return NULL;

    printf("Memory allocated");

    node->localisation = localisation;
    node->cost = cost;
    node->parent = NULL;
    node->possibilities = 0;
    node->children = NULL;

    return node;
}


void add_child(t_node* parent, t_node* child) {
    if (!parent || !child) return;

    child->parent = parent;

    parent->children = (t_node**)realloc(parent->children, sizeof(t_node*) * (parent->possibilities + 1));
    if (!parent->children) return;


    parent->children[parent->possibilities] = child; //+1 child
    parent->possibilities++;
}





