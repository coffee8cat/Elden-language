#ifndef _EXCHANGE_TREE_H__
#define _EXCHANGE_TREE_H__


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "elden_tree_data.h"
#include "comp_tree.h"
#include "lexical_analysis.h"
#include "id_table.h"
#include "tree_dump.h"

void save_tree(node_t* root, identificator* ids_table, FILE* output);
void read_tree(node_t* root, identificator* ids_table, FILE* input);

#endif
