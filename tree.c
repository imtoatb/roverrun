#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
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

t_tree* initialize_tree_with_choices()
{
    // Create root node
    t_tree* tree = create_tree((t_position){0, 0}, MOVE_NONE, 0); // Root cost is 0

    // Add three child nodes with different costs and positions for testing
    t_node* child1 = create_node((t_position){1, 1}, MOVE_LEFT, 10);
    t_node* child2 = create_node((t_position){1, 2}, MOVE_RIGHT, 5);
    t_node* child3 = create_node((t_position){2, 1}, MOVE_UP, 15);

    // Attach children to the root
    add_child(tree->tree->children[0], child1);
    add_child(tree->tree->children[0], child2);
    add_child(tree->tree->children[0], child3);

    // For testing purposes, add child nodes to each of these nodes as well
    add_child(child1, create_node((t_position){2, 2}, MOVE_LEFT, 8));
    add_child(child2, create_node((t_position){2, 3}, MOVE_RIGHT, 3));
    add_child(child3, create_node((t_position){3, 1}, MOVE_UP, 20));

    return tree;
}

t_node* find_minimum_cost_leaf(t_node* node, int* min_cost, t_node** min_leaf)
{
    if (node->possibilities == 0) // Leaf node
    {
        if (node->cost < *min_cost)
        {
            *min_cost = node->cost;
            *min_leaf = node;
        }
    }
    else
    {
        for (int i = 0; i < node->possibilities; i++)
        {
            find_minimum_cost_leaf(node->children[i], min_cost, min_leaf);
        }
    }
    return *min_leaf;
}

void trace_path_to_leaf(t_node* leaf)
{
    if (leaf == NULL) return;
    trace_path_to_leaf(leaf->parent); // Recursive call to trace up to the root
    printf("Position: (%d, %d), Cost: %d\n", leaf->loc.x, leaf->loc.y, leaf->cost);
}

int main()
{
    t_tree* tree = initialize_tree_with_choices();

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

    return 0;
}
