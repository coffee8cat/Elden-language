#include "backend.h"
#define COMPILER_ERROR(expected)                                                    \
    fprintf(stderr, "\n\n!!! COMPILER ERROR !!!\n"                                  \
                    "In %s:%d in %s expected <%s>\n",                               \
                    __FILE__, __LINE__, __func__, #expected);                       \
    assert(0)                                                                       \

void prepare_to_compile(node_t* root, identificator* ids_table)
{
    assert(root);
    assert(ids_table);

    if (root -> type == OP && root -> value == FUNCTION_DEFINITION)
    {
        write_func_def_info(root, ids_table);
        return;
    }
    else if (root -> type == OP && root -> value == CALL)
    {
        return;
    }
    else if (root -> type == ID)
    {
        write_var_info(root, ids_table);
        return;
    }

    if (root -> left)  { prepare_to_compile(root -> left, ids_table);  }
    if (root -> right) { prepare_to_compile(root -> right, ids_table); }
}

void write_func_def_info(node_t* root, identificator* ids_table)
{
    assert(root);
    assert(root -> left);
    assert(root -> left -> left);
    assert(ids_table);

    if (root -> left.type == OP && root -> left.value == FUNCTION_SPECIFICATION)
    {
        if ((root -> left -> left).type == ID)
        {
            ids_table[root -> value.id].num_of_params   = count_params(root -> left -> left);
            ids_table[root -> value.id].is_defined      = true;
        }
        else
        {
            fprintf(stderr, "ERROR: id expected in LLINK of function specification node[%p]\n", root -> left);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: specification expected in LLINK of function definition node[%p]\n", root);
    }
}

size_t count_params(node_t* node, identificator* ids_table)
{
    assert(node);
    assert(ids_table);

    size_t num_of_params = 0;
    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> left)  { num_of_params += mark_local_vars(node -> left, ids_table);  }
        if (node -> right) { num_of_params += mark_local_vars(node -> right, ids_table); }
        return num_of_params;
    }
    else if (node -> type == ID)
    {
        ids_table[node -> value.id].scope   = LOCAL;
        ids_table[node -> value.id].address = num_of_params;
        num_of_params++;
        return num_of_params;
    }
    else { assert(0); }
}

void translate_OP(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    if (curr -> type == OP)
    {
        switch (curr -> value.op)
        {
            case BOND:
            {
                translate_OP(curr -> left,  ids_table, output);
                translate_OP(curr -> right, ids_table, output);
                break;
            }
            case ASSIGNMENT:    { translate_Assignment(curr, ids_table, output); break; }
            case IF:            { translate_IF(curr, ids_table, output);         break; }
            case WHILE:         { translate_While(curr, ids_table, output);      break; }
            case RTN:           { translate_Return(curr, ids_table, output);     break; }

            case FUNCTION_DEFINITION:   { translate_Function_Definition(curr, ids_table, output);  break; }
            case CALL:                  { translate_Function_Call(curr, ids_table, output);        break; }

            default: { fprintf(stderr, "ERROR: No such operation\n"); }
        }
    }
    else
    {
        fprintf()
    }
}

void translate_IF(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    static size_t IF_counter = 0;

    fprintf(output, "\n; IF\n; condition\n");
    translate_expression(curr -> left, ids_table, output);

    fprintf(output, "\n; test condition\n")
    fprintf(output, "PUSH 0\n JA endif%d:", IF_counter);

    fprintf(output, "\n; body\n");
    translate_OP(curr -> right, ids_table, output);

    fprintf(output, "\nendif%d:\n", IF_counter);

    IF_counter++;
}

void translate_While(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    static size_t while_counter = 0;

    fprintf(output, "\n; WHILE\n"
                    "while_start%d:\n"
                    "; condition\n");

    translate_expression(curr -> left, ids_table, output);

    fprintf(output, "\n; test condition\n")
    fprintf(output, "PUSH 0\n JA end_while%d:", while_counter); // change to JE

    fprintf(output, "\n; body\n");
    translate_OP(curr -> right, ids_table, output);

    fprintf(output, "\nJMP while_start%d:",  while_counter);
    fprintf(output, "\nendwhile%d:\n",       while_counter);

    while_counter++;

}

void translate_Function_Definition(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(curr -> left);
    assert(curr -> right);
    assert(ids_table);
    assert(output);

    fprintf(output, "PUSH BX\n");

    //параметры функции лежат в стэке в нужном порядке
    get_params(curr -> left, ids_table, output);

    // больше проверок на типы узлов

    translate_OP(curr -> right);
}

void get_params(node_t* func_spec_node, identificator* ids_table, FILE* output)
{
    assert(func_spec_node);
    assert(ids_table);
    assert(output);

    for (size_t i = 0; i < ids_table[(func_spec_node -> left) -> value.id].num_of_params; i++)
    {
        fprintf(output, "POP [BX+%d]\n", i);
    }
    fprintf(output, "PUSH %d\n"
                    "PUSH BX\n"
                    "ADD\n"
                    "POP BX\n", ids_table[(func_spec_node -> left) -> value.id].num_of_params);
}

void translate_Return(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    translate_Expression(curr -> left, ids_table, output);
    fprintf("POP AX\n"
            "POP BX\n"
            "RTN\n");
}

void translate_Function_Call(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    // current is node with OP CALL

    if (not ids_table[curr -> left -> value.id].is_defined)
    {
        fprintf(stderr, "ERROR: Function %.*s was not defined in this scope\n",
                        ids_table[curr -> left -> value.id].name,
                        ids_table[curr -> left -> value.id].name_len);
    }

    push_call_params(curr -> right, ids_table, output, html_stream);

    if (curr -> type != ID) { COMPILER_ERROR(ID type node);}

    fprintf(output, "\nCALL %s:\n", ids_table[curr -> value.id]);
}

void push_call_params(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> right) { push_call_params(node -> right, ids_table, output, html_stream); }
        if (node -> left)  { push_call_params(node -> left,  ids_table, output, html_stream); }
    }
    else if (node -> type == ID)
    {
        translate_node_value(node, ids_table, output, html_stream);
    }
    else { assert(0); }
}

#define DEF_OPERATION(enum_name, dump_name) #enum_name,

const char* proc_operations_list[] =
{
    #include "DSL_elden_operations.h"
};
#undef DEF_OPERATION

void translate_Expression(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    if (curr -> type == OP)
    {
        translate_Expression(curr -> left,  ids_table, output);
        translate_Expression(curr -> right, ids_table, output);
        fprintf(output, "\n%s\n", proc_operations_list[curr -> value.op]);
    }
    else
    {
        translate_node_value(curr, ids_table, output);
    }
}

void translate_node_value(node_t* curr, identificator* ids_table, FILE* output)
{
    assert(curr);
    assert(ids_table);
    assert(output);

    fprintf(output, "PUSH ");
    if (curr -> type == ID)
    {
        if (ids_table[curr -> value.id].scope == LOCAL) { fprintf(output, "[BX+%d]\n", ids_table[curr -> value.id].address); }
        else                                            { fprintf(output, "[%d]\n",    ids_table[curr -> value.id].address); }
    }
    else if (curr -> type == NUM)
    {
        fprintf(output, "%d\n", curr -> value.num);
    }
}
