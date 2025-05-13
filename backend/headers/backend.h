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
#include "files_usage.h"

typedef void (elden_to_asm_translation_func)(node_t*, identificator*, FILE*);

void tree_to_asm(node_t* node, identificator* ids_table, size_t BX_shift);
void fill_global_vars(identificator* ids_table, FILE* output);
void translate_OP(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);

void asm_translate_Assignment   (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);

void translate_condition(FILE* output);

void asm_translate_IF           (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void asm_translate_While        (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);

void asm_translate_Function_Definition(node_t* node, identificator* ids_table, FILE* output);
//elden_to_asm_translation_func get_params;

void asm_translate_Return       (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void asm_translate_Function_Call(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void asm_push_call_params       (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);

void asm_translate_Print        (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void asm_translate_Scan         (node_t* node, identificator* ids_table, FILE* output);
//elden_to_asm_translation_func translate_Scan;


void asm_translate_Expression   (node_t* node, identificator* ids_table, FILE* output, size_t BX_shift);
void asm_translate_push_node_value (node_t* node, identificator* ids_table, FILE* output);
void asm_translate_pop_var         (node_t* node, identificator* ids_table, FILE* output);
//elden_to_asm_translation_func asm_translate_push_node_value;
//elden_to_asm_translation_func translate_pop_var;

#endif
