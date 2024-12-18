#include "exchange_tree.h"

static size_t tabs_shift = 0;

#define PRINT_TABS_SHIFT()                                      \
    for (size_t i = 0; i < tabs_shift; i++)                     \
    {                                                           \
        fprintf(output, "\t");                                  \
        printf("\t");                                           \
    }                                                           \

#define PRINT_OUT(...)                                          \
    PRINT_TABS_SHIFT()                                          \
    fprintf(output, __VA_ARGS__);                               \
    printf(__VA_ARGS__)                                         \

void save_tree(node_t* node, identificator* ids_table, FILE* output)
{
    if (not node) { return; }

    assert(ids_table);
    assert(output);


    if (node -> type == OP)
    {
        if (node -> value.op == VAR_DEFINITION)
        {

            PRINT_OUT("{OP:\"=\"\n");
            tabs_shift++;
            save_tree(node -> left,  ids_table, output);
            PRINT_OUT("{NUM:\"0\"}\n");
            tabs_shift--;
            PRINT_OUT("}\n");
            return;
        }
        if (node -> value.op == FUNCTION_SPECIFICATION)
        {
            PRINT_OUT("{OP:\"spec\"\n");
            tabs_shift++;
            save_tree   (node -> left,  ids_table, output);
            save_params (node -> right, ids_table, output);
            tabs_shift--;
            PRINT_OUT("}\n");
            return;
        }
        switch (node -> value.op)
        {

            #define DEF_OPERATION(name, name_for_save)                  \
                case name:                                              \
                {                                                       \
                    PRINT_OUT("{OP:\"%s\"\n", name_for_save);           \
                    tabs_shift++;                                       \
                    save_tree(node -> left,  ids_table, output);        \
                    save_tree(node -> right, ids_table, output);        \
                    tabs_shift--;                                       \
                    PRINT_OUT("}\n");                                   \
                    break;                                              \
                }                                                       \

            #include "DSL_elden_operations.h"
            #undef DEF_OPERATION

            default: { fprintf(stderr, "ERROR: No such operation\n"); }
        }
    }
    else
    {
        if (node -> type == ID)
        {
            PRINT_OUT("{ID:\"%.*s\"}\n", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
        }
        else if (node -> type == NUM)
        {
            PRINT_OUT("{NUM:\"%lg\"}\n", node -> value.num);
        }
        else
        {
            fprintf(stderr, "ERROR: Invalid type: %lg\n", node -> value.num);
        }
    }
}

void save_params(node_t* node, identificator* ids_table, FILE* output)
{
    if (not node) { return; }

    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        PRINT_OUT("{OP:\",\"\n");
        tabs_shift++;
        save_params(node -> left,  ids_table, output);
        save_params(node -> right, ids_table, output);
        tabs_shift--;
        PRINT_OUT("}\n");
    }
    else
    {
        if (node -> type == ID)
        {
            PRINT_OUT("{ID:\"%.*s\"}\n", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
        }
        else if (node -> type == NUM)
        {
            PRINT_OUT("{NUM:\"%lg\"}\n", node -> value.num);
        }
        else
        {
            fprintf(stderr, "ERROR: Invalid type: %lg\n", node -> value.num);
        }
    }
}

void read_tree(node_t* node, identificator* ids_table, FILE* input)
{
    assert(node);
    assert(ids_table);
    assert(input);

    printf("Sorry this feature is in development yet(\n");
    //nothing yet
}
