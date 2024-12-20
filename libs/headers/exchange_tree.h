#ifndef _EXCHANGE_TREE_H__
#define _EXCHANGE_TREE_H__


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "elden_tree_data.h"
#include "comp_tree.h"
#include "id_table.h"
#include "tree_dump.h"

void save_tree_data(node_t* node, identificator* ids_table);
void save_tree(node_t* root, identificator* ids_table, FILE* output);
void save_params(node_t* node, identificator* ids_table, FILE* output);

node_t* read_tree_data(identificator* ids_table, const char** curr, FILE* html_stream);
bool check_signatures(const char** curr);
node_t* read_tree(identificator* ids_table, const char** curr);
node_t* read_node(identificator* ids_table, const char** curr);

void skip_spaces(const char** curr);

void change_on_defs(node_t** node);

#endif
