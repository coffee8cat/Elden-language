#include "backend.h"
#define COMPILER_ERROR(expected)                                                    \
    fprintf(stderr, "\n\n!!! COMPILER ERROR !!!\n"                                  \
                    "In %s:%d in %s expected <%s>\n",                               \
                    __FILE__, __LINE__, __func__, #expected);                       \
    assert(0)                                                                       \

void translate_OP(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        switch (node -> value.op)
        {
            case BOND:
            {
                translate_OP(node -> left,  ids_table, output);
                translate_OP(node -> right, ids_table, output);
                break;
            }
            case ASSIGNMENT:    { translate_Assignment  (node, ids_table, output); break; }
            case IF:            { translate_IF          (node, ids_table, output); break; }
            case WHILE:         { translate_While       (node, ids_table, output); break; }
            case RTN:           { translate_Return      (node, ids_table, output); break; }

            case FUNCTION_DEFINITION:   { translate_Function_Definition (node, ids_table, output); break; }
            case CALL:                  { translate_Function_Call       (node, ids_table, output); break; }

            case ELEM_IN:  { translate_Scan  (node, ids_table, output); break;}
            case ELEM_OUT: { translate_Print (node, ids_table, output); break;}

            default: { fprintf(stderr, "ERROR: No such operation\n"); }
        }
    }
    else
    {
        fprintf(stderr, "ERROR: node of OP type expected\n");
    }
}

void translate_Assignment(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n; Assignment\n");
    translate_Expression(node -> right, ids_table, output);
    translate_pop_var(node -> left, ids_table, output);
    fprintf(output, "\n; Assignment End\n");
}

void translate_IF(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    static size_t IF_counter = 0;

    fprintf(output, "\n; IF\n; condition\n");
    translate_Expression(node -> left, ids_table, output);

    fprintf(output, "\n; test condition\n");
    fprintf(output, "PUSH 0\n JA endif%d:", IF_counter);

    fprintf(output, "\n; body\n");
    translate_OP(node -> right, ids_table, output);

    fprintf(output, "\nendif%d:\n", IF_counter);

    IF_counter++;
}

void translate_While(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    static size_t while_counter = 0;

    fprintf(output, "\n; WHILE\n"
                    "while_start%d:\n"
                    "; condition\n", while_counter);

    translate_Expression(node -> left, ids_table, output);

    fprintf(output, "\n; test condition\n");
    fprintf(output, "PUSH 0\n JA end_while%d:", while_counter); // change to JE

    fprintf(output, "\n; body\n");
    translate_OP(node -> right, ids_table, output);

    fprintf(output, "\nJMP while_start%d:",  while_counter);
    fprintf(output, "\nendwhile%d:\n",       while_counter);

    while_counter++;

}

void translate_Function_Definition(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(node -> left);
    assert(node -> right);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n; FUNCTION DEFINITION\n");
    fprintf(output, "%.*s:\n\n", ids_table[node -> left -> left -> value.id].name_len, ids_table[node -> left -> left -> value.id].name);
    fprintf(output, "PUSH BX\n");

    //параметры функции лежат в стэке в нужном порядке
    get_params(node -> left, ids_table, output);

    // больше проверок на типы узлов

    translate_OP(node -> right, ids_table, output);
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
                    "POP BX\n", ids_table[(func_spec_node -> left) -> value.id].BX_shift);
}

void translate_Return(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n; RETURN\n");
    translate_Expression(node -> left, ids_table, output);
    fprintf(output, "POP AX\n"
                    "POP BX\n"
                    "RTN\n");
}

void translate_Function_Call(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, ";CALL\n");
    // nodeent is node with OP CALL

    // check function
    if (not ids_table[node -> left -> value.id].is_defined)
    {
        fprintf(stderr, "ERROR: Function %.*s was not defined in this scope\n",
                        ids_table[node -> left -> value.id].name_len,
                        ids_table[node -> left -> value.id].name);
    }

    push_call_params(node -> right, ids_table, output);

    //if (node -> right -> type != ID) { COMPILER_ERROR(ID type node);}

    fprintf(output, "\nCALL %.*s:\n", ids_table[node -> left -> value.id].name_len, ids_table[node -> left -> value.id].name);
    fprintf(output, "PUSH AX\n");
    fprintf(output, "; CALL END\n");
}

void push_call_params(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> right) { push_call_params(node -> right, ids_table, output); }
        if (node -> left)  { push_call_params(node -> left,  ids_table, output); }
    }
    else if (node -> type == ID)
    {
        translate_push_node_value(node, ids_table, output);
    }
    else { assert(0); }
}

void translate_Print(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "ELEM_OUT\n");
}

void translate_Scan(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "ELEM_IN\n");
}

#define DEF_OPERATION(enum_name, dump_name) #enum_name,

const char* proc_operations_list[] =
{
    #include "DSL_elden_operations.h"
};
#undef DEF_OPERATION

void translate_Expression(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        translate_Expression(node -> left,  ids_table, output);
        translate_Expression(node -> right, ids_table, output);

        // TODO: Check if it is math function???
        if (node -> value.op == CALL)
        {
            translate_Function_Call(node, ids_table, output);
        }
        else
        {
            fprintf(output, "\n%s\n", proc_operations_list[node -> value.op]);
        }
    }
    else
    {
        translate_push_node_value(node, ids_table, output);
    }
}

void translate_push_node_value(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "PUSH ");
    if (node -> type == ID)
    {
        if (ids_table[node -> value.id].scope == LOCAL) { fprintf(output, "[BX+%d]\n", ids_table[node -> value.id].address); }
        else                                            { fprintf(output, "[%d]\n",    ids_table[node -> value.id].address); }
    }
    else if (node -> type == NUM)
    {
        fprintf(output, "%lf\n", node -> value.num);
    }
}

void translate_pop_var(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "POP ");
    if (node -> type == ID)
    {
        if (ids_table[node -> value.id].scope == LOCAL) { fprintf(output, "[BX+%d]\n", ids_table[node -> value.id].address); }
        else                                            { fprintf(output, "[%d]\n",    ids_table[node -> value.id].address); }
    }
    else
    {
        fprintf(stderr, "ERROR: node fo ID type expected\n");
    }
}
