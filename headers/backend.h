#ifndef _BACKEND_TRANSLATION_TO_ASSEMBLY_H__
#define _BACKEND_TRANSLATION_TO_ASSEMBLY_H__


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "elden_tree_data.h"
#include "comp_tree.h"
#include "id_table.h"
#include "tree_dump.h"


void prepare_to_compile (node_t* root, identificator* ids_table);
void write_func_def_info(node_t* root, identificator* ids_table);
size_t count_params     (node_t* node, identificator* ids_table);

typedef void (elden_to_asm_translation_func)(node_t*, identificator*, FILE*);

elden_to_asm_translation_func translate_OP;

elden_to_asm_translation_func translate_IF;
elden_to_asm_translation_func translate_While;

elden_to_asm_translation_func translate_Function_Definition;
elden_to_asm_translation_func get_params;
elden_to_asm_translation_func translate_Return;
elden_to_asm_translation_func translate_Function_Call;
elden_to_asm_translation_func push_call_params;

elden_to_asm_translation_func translate_Expression;
elden_to_asm_translation_func translate_Var_Usage;

#endif
