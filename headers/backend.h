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

typedef void (elden_to_asm_translation_func)(node_t*, identificator*, FILE*);


void translate_OP(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);

void translate_Assignment   (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void translate_IF           (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void translate_While        (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);

elden_to_asm_translation_func translate_Function_Definition;
elden_to_asm_translation_func get_params;

void translate_Return       (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void translate_Function_Call(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
elden_to_asm_translation_func push_call_params;

elden_to_asm_translation_func translate_Print;
elden_to_asm_translation_func translate_Scan;


void translate_Expression   (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
elden_to_asm_translation_func translate_push_node_value;
elden_to_asm_translation_func translate_pop_var;

#endif
