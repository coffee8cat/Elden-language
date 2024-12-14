#ifndef _COMPILER_TREE_H__
#define _COMPILER_TREE_H__

#include "elden_tree_data.h"

node_t* new_node(node_type type, node_value value, node_t* left, node_t* right);
void tree_dtor(node_t* node);

#endif
