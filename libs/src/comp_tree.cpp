#include "comp_tree.h"

node_t* new_node(node_type type, node_value value, node_t* left, node_t* right)
{
    node_t* node = (node_t*)calloc(1, sizeof(node_t));
    if (!node)
    {
        fprintf(stderr, "ERROR: Calloc returned NULL in new_node()");
        return NULL;
    }
    node -> type  = type;
    node -> value = value;
    node -> left  = left;
    node -> right = right;

    return node;
}

void tree_dtor(node_t* node)
{
    if (!node) { return; }
    tree_dtor(node -> left);
    tree_dtor(node -> right);
    free(node);
}
