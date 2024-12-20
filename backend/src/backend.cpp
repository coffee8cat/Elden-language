#include "backend.h"

static size_t IF_counter = 0;

#define COMPILER_ERROR(expected)                                                    \
    fprintf(stderr, "\n\n!!! COMPILER ERROR !!!\n"                                  \
                    "In %s:%d in %s expected <%s>\n",                               \
                    __FILE__, __LINE__, __func__, #expected);                       \
    assert(0)                                                                       \

void tree_to_asm(node_t* node, identificator* ids_table, size_t BX_shift)
{
    assert(node);
    assert(ids_table);

    printf("number of global vars: %d\n", BX_shift);
    dump_ids_table(ids_table);

    FILE* fp = get_stream_for_save();

    translate_OP(node, ids_table, fp, BX_shift);
    fprintf(fp, "\n\nHLT\n");

    fclose(fp);
}

void translate_OP(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    if (!node) { return; }
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        switch (node -> value.op)
        {
            case BOND:
            {
                translate_OP(node -> left,  ids_table, output, BX_shift);
                translate_OP(node -> right, ids_table, output, BX_shift);
                break;
            }
            case ASSIGNMENT:    { translate_Assignment  (node, ids_table, output, BX_shift); break; }
            case IF:            { translate_IF          (node, ids_table, output, BX_shift); break; }
            case WHILE:         { translate_While       (node, ids_table, output, BX_shift); break; }
            case RTN:           { translate_Return      (node, ids_table, output, BX_shift); break; }

            case FUNCTION_DEFINITION:   { translate_Function_Definition (node, ids_table, output);  break; }
            case CALL:                  { translate_Function_Call       (node, ids_table, output, BX_shift);    break; }

            case ELEM_IN:  { translate_Scan  (node, ids_table, output); break;}
            case ELEM_OUT: { translate_Print (node, ids_table, output, BX_shift); break;}

            case VAR_DEFINITION: { break; }
            default: { fprintf(stderr, "ERROR: No such operation\n"); }
        }
    }
    else
    {
        fprintf(stderr, "ERROR: node of OP type expected\n");
    }
}

void translate_Assignment(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n; Assignment\n");
    translate_Expression(node -> right, ids_table, output, BX_shift);
    translate_pop_var(node -> left, ids_table, output);
    fprintf(output, "\n; Assignment End\n");
}

void translate_IF(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    size_t local_IF_counter = IF_counter;
    IF_counter++;

    fprintf(output, "\n; IF\n; condition\n");
    translate_Expression(node -> left -> right, ids_table, output, BX_shift);
    translate_Expression(node -> left -> left,  ids_table, output, BX_shift);

    fprintf(output, "\n; test condition\n");
    if (node -> left -> value.op == EQUAL)
    {
        fprintf(output, "\nJNE endif%d:", local_IF_counter);
    }
    else if (node -> left -> value.op == GREATER)
    {
        fprintf(output, "\nJA endif%d:", local_IF_counter);
    }
    else { fprintf(stderr, "ERROR: Invalid operation for condition in IF: %d\n", node -> left -> value.op); assert(0); }

    fprintf(output, "\n; body\n");
    translate_OP(node -> right, ids_table, output, BX_shift);

    fprintf(output, "\nendif%d:\n", local_IF_counter);

    IF_counter++;
}

void translate_While(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    static size_t while_counter = 0;

    fprintf(output, "\n; WHILE\n"
                    "while_start%d:\n"
                    "; condition\n", while_counter);

    translate_Expression(node -> left, ids_table, output, BX_shift);

    fprintf(output, "\n; test condition\n");
    fprintf(output, "PUSH 0\n JA end_while%d:", while_counter); // change to JE

    fprintf(output, "\n; body\n");
    translate_OP(node -> right, ids_table, output, BX_shift);

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

    static size_t func_counter = 0;

    fprintf(output, "JMP func%d:\n", func_counter);
    fprintf(output, "\n; FUNCTION DEFINITION\n");
    fprintf(output, "%.*s:\n\n", ids_table[node -> left -> left -> value.id].name_len, ids_table[node -> left -> left -> value.id].name);

    //параметры функции лежат в стэке в нужном порядке
    get_params(node -> left, ids_table, output);

    // больше проверок на типы узлов

    translate_OP(node -> right, ids_table, output, ids_table[node -> left -> left -> value.id].BX_shift);

    fprintf(output, "\nfunc%d:\n", func_counter);
    func_counter++;
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
    /*fprintf(output, "\nPUSH BX\n"
                    "PUSH %d\n"
                    "PUSH BX\n"
                    "ADD\n"
                    "POP BX\n", ids_table[(func_spec_node -> left) -> value.id].BX_shift);
    */
}

void translate_Return(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n; RETURN\n");
    translate_Expression(node -> left, ids_table, output, BX_shift);
    fprintf(output, "POP AX\n"
                    "POP BX\n"
                    "RTN\n");
}

void translate_Function_Call(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);
/*
    PUSH BX

    PUSH BX_shift in current function
    PUSH BX
    ADD
    POP BX

    CALL

    PUSH AX*/

    fprintf(output, "\n;CALL Save current BX\n");
    fprintf(output, "PUSH BX\n\n");

    // check function
    if (not ids_table[node -> left -> left -> value.id].is_defined)
    {
        fprintf(stderr, "ERROR: Function %.*s was not defined in this scope\n",
                        ids_table[node -> left -> left -> value.id].name_len,
                        ids_table[node -> left -> left -> value.id].name);
    }
    fprintf(output, "\n;push call params\n");
    push_call_params(node -> left -> right, ids_table, output, BX_shift);

    fprintf(output, "\n; BX_shift\n");
    fprintf(output, "PUSH %d\n"
                    "PUSH BX\n"
                    "ADD\n"
                    "POP BX\n", BX_shift);

    //if (node -> right -> type != ID) { COMPILER_ERROR(ID type node);}

    fprintf(output, "\nCALL %.*s:\n",
            ids_table[node -> left -> left -> value.id].name_len,
            ids_table[node -> left -> left -> value.id].name);

    fprintf(output, "PUSH AX\n");
    fprintf(output, "; CALL END\n");
}

void push_call_params(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> right) { push_call_params(node -> right, ids_table, output, BX_shift); }
        if (node -> left)  { push_call_params(node -> left,  ids_table, output, BX_shift); }
    }
    else if ( (node -> type == OP) && (node -> value.op >= ADD) && (node -> value.op <= POW) )
    {
        translate_Expression(node, ids_table, output, BX_shift);
    }
    else if ( (node -> type == OP) && (node -> value.op == CALL))
    {
        translate_Function_Call(node, ids_table, output, BX_shift);
    }
    else if (node -> type == ID || node -> type == NUM)
    {
        translate_push_node_value(node, ids_table, output);
    }
    else { assert(0); }
}

void translate_Print(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    translate_Expression(node -> left, ids_table, output, BX_shift);
    fprintf(output, "ELEM_OUT\n");
}

void translate_Scan(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "ELEM_IN\n");
    translate_pop_var(node -> left, ids_table, output);
}

#define DEF_OPERATION(enum_name, dump_name) #enum_name,

const char* proc_operations_list[] =
{
    #include "DSL_elden_operations.h"
};
#undef DEF_OPERATION

void translate_Expression(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        // TODO: Check if it is math function???
        if (node -> value.op == CALL)
        {
            translate_Function_Call(node, ids_table, output, BX_shift);
        }
        else
        {
            translate_Expression(node -> left,  ids_table, output, BX_shift);
            translate_Expression(node -> right, ids_table, output, BX_shift);

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
        fprintf(output, "%lg\n", node -> value.num);
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
