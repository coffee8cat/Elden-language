#include "tree_dump.h"

node_t* new_node(node_type type, node_value value, node_t* left, node_t* right)
{
    node_t* node = (node_t*)calloc(1, sizeof(node_t));
    if (!node)
    {
        fprintf(stderr, "ERROR: Calloc returned NULL in new_node()");
        return NULL;
    }
    node -> type  = type;
    node -> value = value;
    node -> left  = left;
    node -> right = right;

    return node;
}

void fix_parents(node_t* node)
{
    assert(node);
    if (node -> right) {(node -> right) -> parent = node;}
    if (node -> left)  {(node -> left) ->  parent = node;}
}

void fix_tree(node_t* node)
{
    assert(node);
    fix_parents(node);
    if (node -> right) { fix_parents(node -> right); }
    if (node -> left)  { fix_parents(node -> left);  }
}

void tree_dtor(node_t* node)
{
    if (!node) { return; }
    tree_dtor(node -> left);
    tree_dtor(node -> right);
    free(node);
}

double evaluate_tree(node_t* node, variable* vars_table)
{
    assert(node);
    assert(vars_table);

    if (node -> type == NUM) { printf("num: %lg\n", node -> value.num); return node -> value.num; }
    if (node -> type == VAR) { return vars_table[node -> value.var].value; }
    if (node -> type == OP)
    {
        #define DEF_OPER(oper, eval_rule, ...) case oper: printf("oper: %d\n", oper); return eval_rule;
        switch (node -> value.op)
        {
            #include "diff_rules_DSL.h"
            default: fprintf(stderr, "ERROR: No such operation - [%lf]", node -> value.op);
                     break;
        }
        #undef DEF_OPER
    }

    return -1;
}

node_t* differentiate_tree(node_t* node, variable* vars_table, size_t var_num)
{
    assert(node);
    assert(vars_table);

    if (node -> type == NUM) { return _NUM(0);}
    if (node -> type == VAR)
    {
        if (node -> value.var == var_num) { return _NUM(1); }
        else                              { return _NUM(0); }
    }
    if (node -> type == OP)
    {
        #define DEF_OPER(oper, eval_rule, diff_rule, ...) case oper:            \
        {                                                                       \
            node_t* node_diff = NULL;                                           \
            diff_rule                                                           \
                                                                                \
            return node_diff;                                                   \
        }                                                                       \

        switch (node -> value.op)
        {
            #include "diff_rules_DSL.h"
            default: fprintf(stderr, "ERROR: Unknown operation for differentiator - [%lf]", node -> value);
                     break;
        }
        #undef DEF_OPER
    }

    return NULL;
}

void optimize(node_t* node, variable* vars_table)
{
    assert(node);
    assert(vars_table);
    assert(html_stream);

    size_t opt_counter = 1;
    while (opt_counter > 0)
    {
        opt_counter = 0;
        opt_counter += const_folding(node, vars_table, tex_stream, roots_stack, subs_stack);
        opt_counter += remove_neutral_elems(&node, vars_table, tex_stream, roots_stack, subs_stack);
    }
}

size_t const_folding(node_t* node, variable* vars_table)
{
    assert(vars_table);
    if (!node) { return 0; }

    size_t opt_counter = 0;
    if (node -> left)  { opt_counter += const_folding(node -> left,  vars_table); }
    if (node -> right) { opt_counter += const_folding(node -> right, vars_table); }

    if (node -> type == OP && !check_vars(node))
    {
        node -> value.num = evaluate_tree(node, vars_table);
        node -> type  = NUM;

        tree_dtor(node -> left);  node -> left  = NULL;
        tree_dtor(node -> right); node -> right = NULL;
        opt_counter++;

        printf("EVALUATION: [%p] <- %lf", node -> value);
    }

    return opt_counter;
}

size_t remove_neutral_elems(node_t** node, variable* vars_table)
{
    assert(vars_table);
    if (!node || !(*node)) { return 0; }

    size_t opt_counter = 0;
    if ((*node) -> left)  { opt_counter += remove_neutral_elems(&(*node) -> left,  vars_table); }
    if ((*node) -> right) { opt_counter += remove_neutral_elems(&(*node) -> right, vars_table); }

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

#define REPLACE_WITH(son)       \
    node_t* temp = *node;       \
    *node = (*node) -> son;     \
                                \
    temp -> son = NULL;         \
    tree_dtor(temp);            \
    opt_counter++              \

#define IS_NUM(direction, x) (((*node) -> direction) -> type == NUM && ((*node) -> direction) -> value.num == (x))

size_t ADD_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if      (IS_NUM(left,  0)) { REPLACE_WITH(right); }
    else if (IS_NUM(right, 1)) { REPLACE_WITH(left);  }

    return opt_counter;
}

size_t SUB_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if (IS_NUM(right, 0)) { REPLACE_WITH(left); }

    return opt_counter;
}

size_t MUL_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if      (IS_NUM(left,  0)) { REPLACE_WITH(left);  }
    else if (IS_NUM(left,  1)) { REPLACE_WITH(right); }
    else if (IS_NUM(right, 0)) { REPLACE_WITH(right); }
    else if (IS_NUM(right, 1)) { REPLACE_WITH(left);  }

    return opt_counter;
}

size_t POW_optimisation(node_t** node)
{
    assert(node);
    assert(*node);

    size_t opt_counter = 0;
    if (IS_NUM(left, 0) || IS_NUM(left, 1)) { REPLACE_WITH(left); }
    else if (IS_NUM(right, 0))
    {
        ((*node) -> right) -> type = NUM;
        ((*node) -> right) -> value.num = 1;
        REPLACE_WITH(right);
    }
    else if (IS_NUM(right, 1)) { REPLACE_WITH(left); }

    return opt_counter;
}

#undef REPLACE_WITH
#undef IS_NUM

node_t* copy_tree(node_t* root)
{
    if (!root) { return NULL; }

    node_t* node = new_node(root -> type, root -> value, NULL, NULL); // check
    if (root -> left)  { node -> left  = copy_tree(root -> left);  }
    if (root -> right) { node -> right = copy_tree(root -> right); }

    return node;
}

bool check_vars(node_t* node)
{
    if (!node) { return false;}

    if (node -> type == VAR) { return true; }
    else {return (check_vars(node -> left) || check_vars(node -> right)); }
}

