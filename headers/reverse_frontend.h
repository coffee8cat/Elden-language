#ifndef _REVERSE_FRONTEND_H__
#define _REVERSE_FRONTEND_H__

#include "elden_tree_data.h"
#include "id_table.h"
#include "comp_tree.h"
#include "lexical_analysis.h"
#include "tree_dump.h"

typedef void (reverse_func_t)(node_t*, identificator*, FILE*);

void reverse_frontend(node_t* root, identificator* ids_table);
reverse_func_t reverse_node;
reverse_func_t reverse_Assignment;
reverse_func_t reverse_IF;
reverse_func_t reverse_While;
reverse_func_t reverse_Return;

reverse_func_t reverse_Function_Definition;
reverse_func_t reverse_Function_Call;
reverse_func_t reverse_specification;
reverse_func_t reverse_params;

reverse_func_t reverse_Scan;
reverse_func_t reverse_Print;

reverse_func_t reverse_Expression;

reverse_func_t reverse_Var_Definition;
reverse_func_t reverse_node_value;
reverse_func_t reverse_Var;
#endif
