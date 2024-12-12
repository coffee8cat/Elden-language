#include "lexical_analysis.h"

const char* skip_until_spaces(const char* curr)
{
    assert(curr);
    while(isalpha(*curr) || isdigit(*curr))
        curr++;

    return curr;
}

lexeme_t* string_to_lexems(const char* s, identificator* ids_table)
{
    assert(s);
    lexeme_t* lexeme_array = (lexeme_t*)calloc(1000, sizeof(lexeme_t));
    if (!lexeme_array)
    {
        fprintf(stderr, "ERROR: Calloc for lexeme_array failed\n");
        return NULL;
    }

    const char* curr = s;
    const char* buff_end  = strchr(s, '\0');
    printf("--- curr[%p] end[%p]\n", curr, buff_end);
    size_t lexeme_num = 0;

    while (curr < buff_end)
    {
        printf("curr: %c(%d), num = %d\n", *curr, *curr, lexeme_num);

        if (get_OP(lexeme_array, lexeme_num, &curr) != UNKNOWN)
        {
            lexeme_num++;
            continue;
        }
        else if (get_NUM(lexeme_array, lexeme_num, &curr) != -1)
        {
            lexeme_num++;
            continue;
        }
        else
        {
            if (isalpha(*curr))
            {
                add_label(lexeme_array, lexeme_num, &curr, ids_table);
                lexeme_num++;
                printf("lexeme_num: %d", lexeme_num);
                continue;
            }
        }

        fprintf(stderr, "LEXICAL ERROR: No such lexical rule");
        return NULL;
    }

    return lexeme_array;
}

// TOO MANY FUNCTIONS FOR GET ... !!!

enum operations get_OP(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr)
{
    assert(lexeme_array);
    assert(curr);
    assert(*curr);

    #define DEF_GRAMMAR_TOKEN(name, oper)
    #define DEF_NUMBER_TOKEN(name, oper)

    #define DEF_OPERATION_TOKEN(name, oper)                                 \
    if (strncmp(*curr, name, strlen(name)) == 0)                            \
    {                                                                       \
        printf("oper: %s\n", name);                                         \
        *curr = *curr + strlen(name);                                       \
        lexeme_array[lexeme_num].type  = OP;                                \
        lexeme_array[lexeme_num].value = oper;                              \
        return oper;                                                        \
    }                                                                       \

    #include "DSL_elden.h"
    #undef DEF_GRAMMAR_TOKEN
    #undef DEF_OPERATION_TOKEN
    #undef DEF_NUMBER_TOKEN

    return UNKNOWN;
}

int get_NUM(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr)
{
    assert(lexeme_array);
    assert(curr);
    assert(*curr);

    #define DEF_GRAMMAR_TOKEN(name, value)
    #define DEF_OPERATION_TOKEN(name, value)

    #define DEF_NUMBER_TOKEN(name, value)                                   \
    if (strncmp(*curr, name, strlen(name)) == 0)                            \
    {                                                                       \
        printf("oper: %s\n", name);                                         \
        *curr = *curr + strlen(name);                                       \
        lexeme_array[lexeme_num].type  = NUM;                               \
        lexeme_array[lexeme_num].value = value;                             \
        return (int)value;                                                  \
    }                                                                       \

    #include "DSL_elden.h"
    #undef DEF_GRAMMAR_TOKEN
    #undef DEF_OPERATION_TOKEN
    #undef DEF_NUMBER_TOKEN

    return -1;
}

void add_label(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr, identificator* ids_table)
{
    assert(lexeme_array);
    assert(curr);
    assert(*curr);

    printf("---ADDING LABEL---\n");

    const char* label_end = skip_until_spaces(*curr);
    size_t position = add_variable(ids_table, *curr, (size_t)(label_end - *curr));

    lexeme_array[lexeme_num].type  = ID;
    lexeme_array[lexeme_num].value = position;

    *curr = label_end;
}
