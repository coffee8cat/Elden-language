#include "lexical_analysis.h"

lexeme_t* string_to_lexemes(const char* s, identificator* ids_table)
{
    assert(s);
    lexeme_t* lexeme_array = (lexeme_t*)calloc(1000, sizeof(lexeme_t));
    if (!lexeme_array)
    {
        fprintf(stderr, "ERROR: Calloc for lexeme_array failed\n");
        return NULL;
    }

    const char* curr = s;
    const char* buff_end  = 1 + strchr(s, '$'); // check buff end better!!!
    printf("--- curr[%p] end[%p]\n", curr, buff_end);
    size_t lexeme_num = 0;

    while (curr < buff_end)
    {
        printf("[%p] curr: %c(%d), num = %d\n", curr, *curr, *curr, lexeme_num);
        curr = skip_spaces(curr);
        printf("[%p] curr: %c(%d), num = %d\n", curr, *curr, *curr, lexeme_num);

        if ((*curr) == '{' || (*curr) == '}' || (*curr) == '(' || (*curr) == ')' || (*curr) == ',' || (*curr) == ';' || (*curr) == '$')
        {
            lexeme_array[lexeme_num].type  = OP;
            lexeme_array[lexeme_num].value = *curr;
            printf("OP: %c\n", *curr);
            curr++;
            lexeme_num++;
            continue;
        }
        if (get_token(lexeme_array, lexeme_num, &curr))
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
                continue;
            }
        }

        fprintf(stderr, "LEXICAL ERROR: No such lexical rule");
        return NULL;
    }

    return lexeme_array;
}

bool get_token(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr)
{
    assert(lexeme_array);
    assert(curr);
    assert(*curr);

    #define DEF_TOKEN(name, data_type, data_value)                          \
    if (strncmp(*curr, name, strlen(name)) == 0)                            \
    {                                                                       \
        printf("%s: %s\n", #data_type, name);                               \
        *curr = *curr + strlen(name);                                       \
        lexeme_array[lexeme_num].type  = data_type;                         \
        lexeme_array[lexeme_num].value = data_value;                        \
        return true;                                                        \
    }                                                                       \

    #include "DSL_elden.h"
    #undef DEF_TOKEN

    return false;
}

void add_label(lexeme_t* lexeme_array, size_t lexeme_num, const char** curr, identificator* ids_table)
{
    assert(lexeme_array);
    assert(curr);
    assert(*curr);

    printf("---ADDING LABEL---\n");

    const char* label_end = skip_until_spaces(*curr);
    size_t position = add_id(ids_table, *curr, (size_t)(label_end - *curr));

    lexeme_array[lexeme_num].type  = ID;
    lexeme_array[lexeme_num].value = position;

    *curr = label_end;
}

const char* skip_until_spaces(const char* curr)
{
    assert(curr);
    while(isalpha(*curr) || isdigit(*curr))
        curr++;

    return curr;
}

const char* skip_spaces(const char* curr)
{
    assert(curr);
    while(isspace(*curr))
        curr++;

    return curr;
}

#define DEF_TOKEN(name, type, value) #value,

const char* operations_list[] =
{
    #include "DSL_elden.h"
};

#undef DEF_TOKEN

void dump_lexemes_array(lexeme_t* cmds, identificator* ids_table)
{
    assert(cmds);
    assert(ids_table);

    for (size_t i = 0; i < 300; i++)
    {
        if (cmds[i].type == ID)
        {

            printf("%4d, ID: %.*s\nposition = %d\n",
            i, ids_table[cmds[i].value].name_len, ids_table[cmds[i].value].name, cmds[i].value);
        }
        else
        {
            char type_name[5] = {};
            if (cmds[i].type == NUM) { strcpy(type_name, "NUM"); }
            if (cmds[i].type == OP)  { strcpy(type_name, "OP"); }
            if (cmds[i].value < UNKNOWN)
            {
                printf("%4d, %s: %s\n",
                i, type_name, operations_list[cmds[i].value]);
            }
            else
            {
                printf("%4d, %s: %c\n",
                i, type_name, cmds[i].value);
            }
        }
    }
}
