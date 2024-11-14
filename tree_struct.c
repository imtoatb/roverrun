//
// Created by idfcg on 14/11/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include "tree_struct.h"
#include "moves.h"


t_node* create_node(t_position localisation, t_move move, int cost) {

    t_node* node = (t_node*)malloc(sizeof(t_node));                         //Free space
    if (node == NULL) {
        return NULL;                                                            //If node = Null -> Null
    }

    printf("Memory allocated");                                           //Test

    node->localisation = localisation;
    node->cost = cost;
    node->parent = NULL;
    node->possibilities = 0;
    node->children = NULL;

    return node;
}


void add_child(t_node* parent, t_node* child) {
    if (parent == NULL || child == NULL) {                                      //if parent or chil Null -> exit
        return;
    }

    child->parent = parent;
    int new_size = parent->possibilities + 1;
    t_node** new_children = (t_node**)realloc(parent->children, sizeof(t_node*) * new_size);

    if (new_children == NULL) {
        return;
    }

    parent->children = new_children;
    parent->children[parent->possibilities] = child;
    parent->possibilities++;
}


void free_node(t_node* node) {
    if (node == NULL) {                                                         //exit
        return;
    }

    for (int i = 0; i < node->possibilities; i++) {                             //free child
        free_node(node->children[i]);
    }

    free(node->children);                                                       //empty the children array
    free(node);
}


