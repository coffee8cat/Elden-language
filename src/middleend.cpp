#include "middleend.h"

void prepare_to_compile(node_t* node, identificator* ids_table, size_t* global_vars_counter)
{
    assert(node);
    assert(ids_table);
    assert(global_vars_counter);

    if (node -> type == OP && node -> value.op == FUNCTION_DEFINITION)
    {
        printf("WRITING FUNCTION DEFINITION\n");
        write_func_def_info(node, ids_table);
        return;
    }
    else if (node -> type == OP && node -> value.op == CALL)
    {
        return;
    }
    else if (node -> type == OP && node -> value.op== VAR_DEFINITION)
    {
        printf("WRITING VAR INFO\n");
        write_var_def_info(node, ids_table, global_vars_counter);
        return;
    }

    if (node -> left)  { prepare_to_compile(node -> left,  ids_table, global_vars_counter); }
    if (node -> right) { prepare_to_compile(node -> right, ids_table, global_vars_counter); }
}

void write_func_def_info(node_t* node, identificator* ids_table)
{
    assert(node);
    assert(node -> left);
    assert(node -> left -> left);
    assert(ids_table);

    // node - FUNCTION DEFINITION NODE

    // TODO: DSL FOR FCKNG node -> LEFT -> LEFT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


    if (node -> left -> type == OP && node -> left -> value.op == FUNCTION_SPECIFICATION)
    {
        if ((node -> left -> left) -> type == ID)
        {
            size_t num_of_params = 0;
            count_params(node -> left -> right, ids_table, &num_of_params);

            size_t BX_shift = num_of_params;
            write_local_vars(node -> right, ids_table, &BX_shift);

            printf("num of params = %d, BX_shift = %d\n", num_of_params, BX_shift);

            ids_table[node -> left -> left -> value.id].num_of_params = num_of_params;
            ids_table[node -> left -> left -> value.id].BX_shift      = BX_shift;
            ids_table[node -> left -> left -> value.id].is_defined    = true;

        }
        else
        {
            fprintf(stderr, "ERROR: id expected in LLINK of function specification node[%p]\n", node -> left);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: specification expected in LLINK of function definition node[%p]\n", node);
    }
}

size_t count_params(node_t* node, identificator* ids_table, size_t* num_of_params)
{
    assert(node);
    assert(ids_table);
    assert(num_of_params);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> left)  { count_params(node -> left,  ids_table, num_of_params); }
        if (node -> right) { count_params(node -> right, ids_table, num_of_params); }
    }
    else if (node -> type == ID)
    {
        ids_table[node -> value.id].scope   = LOCAL;
        ids_table[node -> value.id].address = *num_of_params;
        (*num_of_params)++;
    }
    else { assert(0); }
}

void write_var_def_info(node_t* node, identificator* ids_table, size_t* global_vars_counter)
{
    assert(node);
    assert(global_vars_counter);

    ids_table[node -> left -> value.id].is_defined  = true;
    ids_table[node -> left -> value.id].scope       = GLOBAL;
    ids_table[node -> left -> value.id].address     = *global_vars_counter;

    (*global_vars_counter)++;
}

size_t write_local_vars(node_t* node, identificator* ids_table, size_t* BX_shift)
{
    assert(node);
    assert(ids_table);

    if (not (node -> type == OP && node -> value.op == VAR_DEFINITION))
    {
        if (node -> left)  { write_local_vars(node -> left,  ids_table, BX_shift); }
        if (node -> right) { write_local_vars(node -> right, ids_table, BX_shift); }
    }
    else
    {
        assert(node -> left);
        ids_table[node -> left -> value.id].scope   = LOCAL;
        ids_table[node -> left -> value.id].address = *BX_shift;

        (*BX_shift)++;
    }
}
