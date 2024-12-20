#ifndef _SYNTAX_ANALYSIS_H__
#define _SYNTAX_ANALYSIS_H__

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

typedef node_t* (syntax_func)(lexeme_t*, identificator*, size_t*, FILE*);
syntax_func get_General;
syntax_func get_Operation;

syntax_func get_IF;
syntax_func get_While;
syntax_func get_Assignment;
syntax_func get_Var_Definition;

syntax_func get_Function_Definition;
syntax_func get_Function_Call;
syntax_func get_Return;
syntax_func get_Scan;
syntax_func get_Print;

syntax_func get_Var;
syntax_func get_Expression;
syntax_func get_T;
syntax_func get_POW;
syntax_func get_Primary;

#endif
