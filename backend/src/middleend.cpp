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

void count_params(node_t* node, identificator* ids_table, size_t* num_of_params)
{
    if (node == NULL) { return; }

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

void write_local_vars(node_t* node, identificator* ids_table, size_t* BX_shift)
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

/*
double evaluate_tree(node_t* node, variable* vars_table)
{
    assert(node);
    assert(vars_table);

    if (node -> type == NUM) { printf("num: %lg\n", node -> value.num); return node -> value.num; }
    if (node -> type == ID)  { return vars_table[node -> value.var].value; }
    if (node -> type == OP)
    {

    }

    return -1;
}

void const_folding(node_t* node, idsntificator* ids_table)
{
    if (!node) { return 0; }
    assert(ids_table);

    size_t opt_counter = 0;
    if (node -> left)  { opt_counter += const_folding(node -> left,  vars_table, tex_stream, roots_stack, subs_stack); }
    if (node -> right) { opt_counter += const_folding(node -> right, vars_table, tex_stream, roots_stack, subs_stack); }

    if (node -> type == OP && node -> value.op >= ADD && !check_vars(node))
    {
        node -> value.num = evaluate_tree(node, vars_table);
        node -> type  = NUM;

        tree_dtor(node -> left,  true);  node -> left  = NULL;
        tree_dtor(node -> right, true);  node -> right = NULL;
        opt_counter++;

        printf("EVALUATION: [%p] <- %lf", node -> value);
    }
}

bool check_vars(node_t* node)
{
    if (!node) { return false;}

    if (node -> type == ID) { return true; }
    else {return (check_vars(node -> left) || check_vars(node -> right)); }
}
*/

/*
void optimize(node_t* node, identificator* ids_table)
{
    assert(node);
    assert(ids_table);

    size_t opt_counter = 1;
    while (opt_counter > 0)
    {
        opt_counter = 0;
        opt_counter += const_folding(node, ids_table);
        opt_counter += remove_neutral_elems(&node, ids_table);
    }
}

size_t const_folding(node_t* node, identificator* ids_table)
{
    assert(ids_table);
    if (!node) { return 0; }

    size_t opt_counter = 0;
    if (node -> left)  { opt_counter += const_folding(node -> left,  ids_table); }
    if (node -> right) { opt_counter += const_folding(node -> right, ids_table); }

    if (node -> type == OP && !check_vars(node))
    {
        node -> value.num = evaluate_tree(node, ids_table);
        node -> type  = NUM;

        tree_dtor(node -> left,  true);  node -> left  = NULL;
        tree_dtor(node -> right, true); node -> right = NULL;
        opt_counter++;

        printf("EVALUATION: [%p] <- %lf", node -> value);
    }

    return opt_counter;
}

size_t remove_neutral_elems(node_t** node, variable* vars_table, FILE* tex_stream, stack_t* roots_stack, stack_t* subs_stack)
{
    assert(vars_table);
    assert(tex_stream);
    assert(roots_stack);
    assert(subs_stack);

    if (!node || !(*node)) { return 0; }

    size_t opt_counter = 0;
    if ((*node) -> left)  { opt_counter += remove_neutral_elems(&(*node) -> left,  vars_table, tex_stream, roots_stack, subs_stack); }
    if ((*node) -> right) { opt_counter += remove_neutral_elems(&(*node) -> right, vars_table, tex_stream, roots_stack, subs_stack); }

    if ((*node) -> type == OP)
    {
        switch((*node) -> value.op)
        {
            case ADD: opt_counter += ADD_optimisation(node); printf("ADD OPTIMISATION\n"); break;
            case SUB: opt_counter += SUB_optimisation(node); printf("SUB OPTIMISATION\n"); break;
            case MUL: opt_counter += MUL_optimisation(node); printf("MUL OPTIMISATION\n"); break;
            case POW: opt_counter += POW_optimisation(node); printf("POW OPTIMISATION\n"); break;
            default: break;
        }
    }

    return opt_counter;
}

#define REPLACE_WITH_RIGHT(node)                \
    printf( "before right replace:\n"           \
            "node:  %p\n"                       \
            "left:  %p\n"                       \
            "right: %p\n\n",                    \
            *node,                              \
            (*node) -> left,                    \
            (*node) -> right);                  \
                                                \
    tree_dtor((*node) -> left, false);          \
    node_t* temp = *node;                       \
    *node = (*node) -> right;                   \
    tree_dtor(temp, false);                     \
                                                \
    if ((*node) -> type != OP)                  \
    {                                           \
        (*node) -> left  = NULL;                \
        (*node) -> right = NULL;                \
    }                                           \
    printf( "after replace:\n"                  \
            "node:  %p\n"                       \
            "left:  %p\n"                       \
            "right: %p\n\n",                    \
            *node,                              \
            (*node) -> left,                    \
            (*node) -> right);                  \
    opt_counter++

#define REPLACE_WITH_LEFT(node)                 \
    printf( "before left replace:\n"            \
            "node:  %p\n"                       \
            "left:  %p\n"                       \
            "right: %p\n\n",                    \
            *node,                              \
            (*node) -> left,                    \
            (*node) -> right);                  \
                                                \
    tree_dtor((*node) -> right, false);         \
    node_t* temp = *node;                       \
    *node = (*node) -> left;                    \
    tree_dtor(temp, false);                     \
                                                \
    if ((*node) -> type != OP)                  \
    {                                           \
        (*node) -> left =  NULL;                \
        (*node) -> right = NULL;                \
    }                                           \
    printf( "after replace:\n"                  \
            "node:  %p\n"                       \
            "left:  %p\n"                       \
            "right: %p\n\n",                    \
            *node,                              \
            (*node) -> left,                    \
            (*node) -> right);                  \
    opt_counter++


#define REPLACE_WITH(son)       \
    node_t* temp = *node;       \
    *node = (*node) -> son;     \
                                \
    temp -> son = NULL;         \
    tree_dtor(temp, true);      \
    opt_counter++               \

#define IS_NUM(direction, x) (((*node) -> direction) -> type == NUM && ((*node) -> direction) -> value.num == (x))

size_t ADD_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if      (IS_NUM(left,  0)) { REPLACE_WITH_RIGHT(node); }
    else if (IS_NUM(right, 1)) { REPLACE_WITH_LEFT(node);  }

    return opt_counter;
}

size_t SUB_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if (IS_NUM(right, 0)) { REPLACE_WITH_LEFT(node); }

    return opt_counter;
}

size_t MUL_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if      (IS_NUM(left,  0)) { REPLACE_WITH_LEFT(node);  }
    else if (IS_NUM(left,  1)) { REPLACE_WITH_RIGHT(node); }
    else if (IS_NUM(right, 0)) { REPLACE_WITH_RIGHT(node); }
    else if (IS_NUM(right, 1)) { REPLACE_WITH_LEFT(node);  }

    return opt_counter;
}

size_t POW_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if (IS_NUM(left, 0) || IS_NUM(left, 1)) { REPLACE_WITH_LEFT(node); }
    else if (IS_NUM(right, 0))
    {
        ((*node) -> right) -> type = NUM;
        ((*node) -> right) -> value.num = 1;
        REPLACE_WITH_RIGHT(node);
    }
    else if (IS_NUM(right, 1)) { REPLACE_WITH_LEFT(node); }

    return opt_counter;
}

#undef REPLACE_WITH
#undef IS_NUM

*/
