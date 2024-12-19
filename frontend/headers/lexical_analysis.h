#ifndef _LEXICAL_ANALYSIS_H__
#define _LEXICAL_ANALYSIS_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "elden_tree_data.h"
#include "id_table.h"

struct lexeme_t
{
    node_type   type;
    size_t     value;
    lexeme_t*   left;
    lexeme_t*  right;
};

lexeme_t* string_to_lexemes(const char* s, identificator* ids_table);
bool get_token(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr);
void add_label(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr, identificator* ids_table);

const char* skip_until_spaces(const char* curr);
const char* skip_spaces(const char* curr);

void dump_lexemes_array(lexeme_t* cmds, identificator* ids_table);

#endif
