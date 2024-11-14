#include <stdio.h>
#include <stdlib.h>
#include "tree_struct.h"
#include "moves.h"

t_node* create_node(t_position loc, t_move move, int cost)
{
    t_node* node = (t_node*)malloc(sizeof(t_node));
    if (node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    node->loc = loc;
    node->cost = cost;
    node->possibilities = 0;
    node->children = NULL;
    node->parent = NULL;
    return node;
}

void add_child(t_node* parent, t_node* child)
{
    if (parent->children == NULL)
    {
        parent->children = (t_node**)malloc(sizeof(t_node*));
        if (parent->children == NULL)
        {
            fprintf(stderr, "Error: malloc failed\n");
            exit(1);
        }
        parent->children[0] = child;
    }
    else
    {
        parent->children = (t_node**)realloc(parent->children, (parent->possibilities + 1) * sizeof(t_node*));
        if (parent->children == NULL)
        {
            fprintf(stderr, "Error: realloc failed\n");
            exit(1);
        }
        parent->children[parent->possibilities] = child;
    }
    parent->possibilities++;
    child->parent = parent;
}

t_tree* create_tree(t_position loc, t_move move, int cost)
{
    t_tree* tree = (t_tree*)malloc(sizeof(t_tree));
    if (tree == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    tree->tree = (t_root*)malloc(sizeof(t_root));
    if (tree->tree == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    tree->tree->children = (t_node**)malloc(sizeof(t_node*));
    if (tree->tree->children == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(1);
    }
    tree->tree->children[0] = create_node(loc, move, cost);
    tree->tree->cost = cost;
    return tree;
}

void free_node(t_node* node)
{
    if (node->children != NULL)
    {
        for (int i = 0; i < node->possibilities; i++)
        {
            free_node(node->children[i]);
        }
        free(node->children);
    }
    free(node);
}

void free_tree(t_tree* tree)
{
    free_node(tree->tree->children[0]);
    free(tree->tree->children);
    free(tree->tree);
    free(tree);
}