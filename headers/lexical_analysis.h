#ifndef _LEXICAL_ANALYSIS_H__
#define _LEXICAL_ANALYSIS_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "id_table.h"

#define DEF_GRAMMAR_TOKEN(name) name,
enum grammar_tokens
{
    #include "DSL_elden.h"
    UNKNOWN
}

enum operations
{
    GRAMMAR,
    IF,
    WHILE,
    ASSIGNMENT,
    VAR_DEFINITION,
    FUNCTION_DEFINITION,
    RETURN,
    FUNCTION_CALL,
    UNKNOWN
}

enum lexeme_type {OP, ID, NUM};
struct lexeme_t
{
    lexeme_type type;
    double     value;
    lexeme_t*   left;
    lexeme_t*  right;
};

const char* skip_until_spaces(const char* curr);
lexeme_t* string_to_lexems(const char* s, identificator* ids_table);
enum operations get_OP(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr);
void add_label(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr, identificator* ids_table);

#endif
