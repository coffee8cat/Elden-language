#include "exchange_tree.h"
#include "../DSL/DSL_comp_tree.h"
#include "files_usage.h"

static size_t tabs_shift = 0;

#define PRINT_TABS_SHIFT()                                      \
    for (size_t i = 0; i < tabs_shift; i++)                     \
    {                                                           \
        fprintf(output, "\t");                                  \
    }                                                           \

#define PRINT_OUT(...)                                          \
    PRINT_TABS_SHIFT()                                          \
    fprintf(output, __VA_ARGS__);                               \

void save_tree_data(node_t* node, identificator* ids_table)
{
    assert(node);
    assert(ids_table);

    FILE* stream_for_save = get_stream_for_save();
    if (!stream_for_save) { fprintf(stderr, "ERROR: Could not open stream_for_save\n"); }

    fprintf(stream_for_save, "434-format v1.5\n");
    save_tree(node, ids_table, stream_for_save);
    fclose(stream_for_save);
}

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

            #define DEF_OPERATION(name, name_for_save, ...)             \
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

            #include "../DSL/DSL_elden_operations.h"
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

    if (node -> type == OP && node -> value.op == BOND)
    {
        PRINT_OUT("{OP:\",\"\n");
        tabs_shift++;
        save_params(node -> left,  ids_table, output);
        save_params(node -> right, ids_table, output);
        tabs_shift--;
        PRINT_OUT("}\n");
    }
    else if (node -> type == OP)
    {
        printf("SAVING OPERATION");
        switch (node -> value.op)
        {
            #define DEF_OPERATION(name, name_for_save, ...)         \
            case name:                                              \
            {                                                       \
                PRINT_OUT("{OP:\"%s\"\n", name_for_save);           \
                tabs_shift++;                                       \
                save_params(node -> left,  ids_table, output);      \
                save_params(node -> right, ids_table, output);      \
                tabs_shift--;                                       \
                PRINT_OUT("}\n");                                   \
                break;                                              \
            }                                                       \

            #include "../DSL/DSL_elden_operations.h"
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

node_t* read_tree_data(identificator* ids_table, const char** curr, FILE* html_stream)
{
    assert(ids_table);
    assert(curr);
    assert(*curr);


    check_signatures(curr);

    node_t* node_read = read_tree(ids_table, curr);

    if (node_read)
    {
        change_on_defs(&node_read);
        dump_ids_table(ids_table);
        tree_dump(node_read, ids_table, html_stream, node_read);
    }
    else { fprintf(stderr, "ERROR: node read from tree is a null_ptr\n"); assert(0); }

    return node_read;
}

bool check_signatures(const char** curr)
{
    assert(curr);
    const char signature[20] = "434-format v1.5";
    if (strncmp(*curr, signature, strlen(signature)) == 0)
    {
        *curr = *curr + strlen(signature);
        return true;
    }
    else { fprintf(stderr, "ERROR: Invalid signature: %.*s\n", strlen(signature), *curr); assert(0); return false; }
}

#define PRINT_CURR() printf("CURRENT on [%.*s]\n", 8, *curr)

node_t* read_tree(identificator* ids_table, const char** curr)
{
    assert(ids_table);
    assert(curr);
    assert(*curr);

    node_t* node = NULL;

    skip_spaces(curr);
    PRINT_CURR();
    if (**curr == '{')
    {
        (*curr)++;
        node = read_node(ids_table, curr);
        printf("type: %d\n", node -> type);
        PRINT_CURR();
        if (node -> type == OP)
        {
            node -> left  = read_tree(ids_table, curr);
        }
        else
        {
            skip_spaces(curr);
            PRINT_CURR();
            if (**curr != '}') { fprintf(stderr, "ERROR: expected [}], got [%c] on [%p] instead", **curr, *curr); assert(0); }
            (*curr)++;
            return node;
        }
    }
    else { return NULL;} // shift????

    node -> right = read_tree(ids_table, curr);

    skip_spaces(curr);
    PRINT_CURR();
    if (**curr != '}') { fprintf(stderr, "ERROR: expected [}], got [%c] on [%p] instead", **curr, *curr); assert(0); }
    (*curr)++;

    return node;
}

node_t* read_node(identificator* ids_table, const char** curr)
{
    assert(ids_table);
    assert(curr);
    assert(*curr);

    printf("Reading node\n");
    PRINT_CURR();

    if (strncmp(*curr, "OP", strlen("OP"))  == 0)
    {
        *curr = strchr(*curr, '\"') + 1;
        PRINT_CURR();
        size_t op_name_len = (size_t)(strchr(*curr, '\"') - *curr);

        if (**curr == ',')
        {
            *curr = *curr + op_name_len + 1;
            return _BOND(NULL, NULL);
        }

        #define DEF_OPERATION(operation, name, ...)                             \
        if (strncmp(*curr, name, op_name_len) == 0)                             \
        {                                                                       \
            *curr = *curr + op_name_len + 1;                                    \
            return _OP(operation, NULL, NULL);                                  \
        }                                                                       \

        #include "../DSL/DSL_elden_operations.h"
        #undef DEF_OPERATION

        fprintf(stderr, "ERROR: No match for operation %.*s\n", op_name_len, *curr);
    }

    if (strncmp(*curr, "ID", strlen("ID"))  == 0)
    {
        PRINT_CURR();
        *curr = strchr(*curr, '\"') + 1;
        size_t id_name_len = (size_t)(strchr(*curr, '\"') - *curr);

        size_t id_num = add_id(ids_table, *curr, id_name_len);

        *curr = strchr(*curr, '\"') + 1;
        return _ID(id_num);
    }

    if (strncmp(*curr, "NUM", strlen("NUM")) == 0)
    {
        PRINT_CURR();
        *curr = strchr(*curr, '\"') + 1;
        double num = 0;

        sscanf(*curr, "%lg\"", &num);
        *curr = strchr(*curr, '\"') + 1;
        PRINT_CURR();
        return _NUM(num);
    }


    fprintf(stderr, "ERROR: Could not read node: %.*s\n", 16, *curr);
    assert(0);
}

void skip_spaces(const char** curr)
{
    assert(curr);
    assert(*curr);

    while(isspace(**curr))
        (*curr)++;
}

void change_on_defs(node_t** node)
{
    if (!*node) { return; }

    if ((*node) -> type == OP && (*node) -> value.op == ASSIGNMENT)
    {
        if ((*node) -> right -> type == NUM && (*node) -> right -> value.num == 0)
        {
            *node = _VAR_DEFINITION((*node) -> left);
        }
    }

    change_on_defs(&((*node) -> left));
    change_on_defs(&((*node) -> right));
}
