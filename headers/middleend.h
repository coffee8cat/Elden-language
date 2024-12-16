#ifndef _MIDDLE_END_H__
#define _MIDDLE_END_H__

#include "elden_tree_data.h"
#include "comp_tree.h"
#include "id_table.h"

void prepare_to_compile   (node_t* node, identificator* ids_table, size_t* global_vars_counter);
void write_func_def_info  (node_t* node, identificator* ids_table);
void count_params         (node_t* node, identificator* ids_table, size_t* num_of_params);
void write_var_def_info   (node_t* node, identificator* ids_table, size_t* global_vars_counter);
void write_local_vars     (node_t* node, identificator* ids_table, size_t* BX_shift);

#endif
