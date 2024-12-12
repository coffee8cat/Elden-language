#ifndef _LEXICAL_ANALYSIS_H__
#define _LEXICAL_ANALYSIS_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "id_table.h"

#define DEF_OPERATION_TOKEN(name, enum_value) enum_value,
#define DEF_NUMBER_TOKEN(name, value)

enum operations
{
    #include "DSL_elden.h"
    UNKNOWN
}

#undef DEF_OPERATION_TOKEN
#undef DEF_NUMBER_TOKEN

enum lexeme_type {OP, ID, NUM};
struct lexeme_t
{
    lexeme_type type;
    size_t     value;
    lexeme_t*   left;
    lexeme_t*  right;
};

const char* skip_until_spaces(const char* curr);
lexeme_t* string_to_lexems(const char* s, identificator* ids_table);
enum operations get_OP(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr);
void add_label(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr, identificator* ids_table);

#endif
