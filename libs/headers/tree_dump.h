#ifndef _TREE_DOT_DUMP_H__
#define _TREE_DOT_DUMP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "elden_tree_data.h"
#include "id_table.h"

FILE* prepare_to_dump();
int tree_dump       (node_t* root, identificator* ids_table, FILE* html_stream, node_t* curr);
int dump_to_html    (node_t* root, char* png_file_name,      FILE* html_stream, node_t* curr);
int make_dot_file   (node_t* root, identificator* ids_table, FILE* fp,          node_t* curr);
int write_node_info (node_t* node, identificator* ids_table, FILE* fp,          node_t* curr);

#endif
