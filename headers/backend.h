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

elden_to_asm_translation_func translate_OP;

elden_to_asm_translation_func translate_Assignment;
elden_to_asm_translation_func translate_IF;
elden_to_asm_translation_func translate_While;

elden_to_asm_translation_func translate_Function_Definition;
elden_to_asm_translation_func get_params;
elden_to_asm_translation_func translate_Return;
elden_to_asm_translation_func translate_Function_Call;
elden_to_asm_translation_func push_call_params;

elden_to_asm_translation_func translate_Expression;
elden_to_asm_translation_func translate_push_node_value;
elden_to_asm_translation_func translate_pop_var;

#endif
