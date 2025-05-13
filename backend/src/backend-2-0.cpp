#include "backend.h"

#define TAB_FPRINTF(stream, ...) fprintf(stream, "\t" __VA_ARGS__)

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

    fprintf(fp, "section .text\n\n");
    fprintf(fp, "__start:\n");
    translate_OP(node, ids_table, fp, BX_shift);

    fprintf(fp, "section .data\n\n");
    fill_global_vars(ids_table, fp);

    fclose(fp);
}

void fill_global_vars(identificator* ids_table, FILE* output) {
    assert(output);
    assert(ids_table);

    size_t curr = 0;

    while (ids_table[curr].name && curr < max_num_of_ids) {
        if (ids_table[curr].scope == GLOBAL) {
            fprintf(output, "\t%.*s dq 0\n", ids_table[curr].name_len, ids_table[curr].name);
        }
        curr++;
    }
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

            case ASSIGNMENT:            { asm_translate_Assignment  (node, ids_table, output, BX_shift); break; }
            case IF:                    { asm_translate_IF          (node, ids_table, output, BX_shift); break; }
            case WHILE:                 { asm_translate_While       (node, ids_table, output, BX_shift); break; }
            case RTN:                   { asm_translate_Return      (node, ids_table, output, BX_shift); break; }

            case FUNCTION_DEFINITION:   { asm_translate_Function_Definition (node, ids_table, output);  break; }
            case CALL:                  { asm_translate_Function_Call       (node, ids_table, output, BX_shift);    break; }

            case ELEM_IN:               { asm_translate_Scan  (node, ids_table, output); break;}
            case ELEM_OUT:              { asm_translate_Print (node, ids_table, output, BX_shift); break;}

            case VAR_DEFINITION:        { break; }

            default:                    { fprintf(stderr, "ERROR: No such operation\n"); }
        }
    }
    else
    {
        fprintf(stderr, "ERROR: node of OP type expected\n");
    }
}

void asm_translate_Assignment(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    TAB_FPRINTF(output, ";calculating expression for assignment\n");
    asm_translate_Expression(node -> right, ids_table, output, BX_shift);                   // stores calculations result in stack

    TAB_FPRINTF(output, "pop  xmm2                   ; assignment to %.*s\n",
                ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
    TAB_FPRINTF(output, "movsd  [rbp + %d], xmm2\n", ids_table[node -> value.id].address);
}

void translate_condition(FILE* output) {
    assert(output);

    //fprintf    (output, "\n\tpop  rcx\n");
    //TAB_FPRINTF(output,     "pop  rdx\n");
    //TAB_FPRINTF(output,     "test rcx, rdx\n\n");


    TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; get result of right subtree from stack\n\t"
                        "add   rsp, 8\n");
    TAB_FPRINTF(output, "movsd xmm1, [rsp]          ; get result of right subtree from stack\n\t"
                        "add   rsp, 8\n");
    TAB_FPRINTF(output, "comisd xmm0, xmm1\n");

    return;
}

void asm_translate_IF(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    size_t local_IF_counter = IF_counter;
    IF_counter++;

    TAB_FPRINTF(output, "\n; IF\n\t; condition\n");

    if (node -> left          == NULL) { perror("Empty condition (nullptr) for IF node\n"); assert(0); }
    if (node -> left -> left  == NULL) { perror("Empty left part of condition (nullptr) for IF node\n"); assert(0); }
    if (node -> left -> right == NULL) { perror("Empty right part of condition (nullptr) for IF node\n"); assert(0); }

    asm_translate_Expression(node -> left -> right, ids_table, output, BX_shift);
    asm_translate_Expression(node -> left -> left,  ids_table, output, BX_shift);

    translate_condition(output);

    if (node -> left -> value.op == EQUAL)
    {
        TAB_FPRINTF(output, "jne .endif%d:\n", local_IF_counter);
    }
    else if (node -> left -> value.op == GREATER)
    {
        TAB_FPRINTF(output, "ja .endif%d:\n", local_IF_counter);
    }
    else { fprintf(stderr, "ERROR: Invalid operation for condition in IF: %d\n", node -> left -> value.op); assert(0); }

    TAB_FPRINTF(output, "; if body:\n");
    translate_OP(node -> right, ids_table, output, BX_shift);

    TAB_FPRINTF(output, ".endif%d:\n", local_IF_counter);

    IF_counter++;
}


void asm_translate_While(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    static size_t while_counter = 0;

    TAB_FPRINTF(output, "\n; WHILE\n"
                        ".while_start%d:\n"
                        "\t; test condition\n", while_counter);

    if ( node -> left) {
        asm_translate_Expression(node -> left, ids_table, output, BX_shift);        // store condition result in stack
        //TAB_FPRINTF(output, "pop rcx\n", while_counter);

        //TAB_FPRINTF(output, "push rcx\n", while_counter);
        //TAB_FPRINTF(output, "push rcx\n", while_counter);                           // store two copies


        TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; get result of right subtree from stack\n\t"
                            "add   rsp, 8\n");
        TAB_FPRINTF(output, "sub rsp, 8\n\t"
                            "movsd [rsp], xmm0          ; save result in stack\n\n");
        TAB_FPRINTF(output, "sub rsp, 8\n\t"
                            "movsd [rsp], xmm0          ; save result in stack\n\n");

    }

    translate_condition(output);
    TAB_FPRINTF(output, "jz .end_while%d\n", while_counter);              // end while if condition is value

    fprintf(output, "\n; body\n");
    if ( node -> right) {
        translate_OP(node -> right, ids_table, output, BX_shift);
    }

    TAB_FPRINTF(output, "jmp .while_start%d\n",  while_counter);
    fprintf    (output, "\n.endwhile%d:\n\n",    while_counter);

    while_counter++;
}


void asm_translate_Function_Definition(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(node -> left);
    assert(node -> right);
    assert(ids_table);
    assert(output);

    // it is assumed that for that moment arguments are stored in stack
    TAB_FPRINTF(output, "ret\n\n");
    if (node -> left && node -> left -> left) {
        fprintf(output, "%.*s:\n", ids_table[node -> left -> left -> value.id].name_len, ids_table[node -> left -> left -> value.id].name);
    }
    else { fprintf(stderr, "ERROR: nullptr in function definition"); }

    translate_OP(node -> right, ids_table, output, ids_table[node -> left -> left -> value.id].BX_shift);
}

void asm_translate_Return(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "; prepare return value\n");
    asm_translate_Expression(node -> left, ids_table, output, BX_shift);

    // restore registers

    TAB_FPRINTF(output, "ret\n\n");
}

void asm_translate_Function_Call(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
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

    TAB_FPRINTF(output, "\n;CALL Save current Base pointer (rbp)\n");
    TAB_FPRINTF(output, "push rbp\n\n");

    // check function
    if (not ids_table[node -> left -> left -> value.id].is_defined)
    {
        fprintf(stderr, "ERROR: Function %.*s was not defined in this scope\n",
                        ids_table[node -> left -> left -> value.id].name_len,
                        ids_table[node -> left -> left -> value.id].name);
    }
    TAB_FPRINTF(output, ";push call params\n");

    asm_push_call_params(node -> left -> right, ids_table, output, BX_shift);
    TAB_FPRINTF(output, "sub  rbp, %4d             ; allocate stack frame for function variables\n", BX_shift);

    //if (node -> right -> type != ID) { COMPILER_ERROR(ID type node);}

    TAB_FPRINTF(output, "call %.*s\n",
            ids_table[node -> left -> left -> value.id].name_len,
            ids_table[node -> left -> left -> value.id].name);

    TAB_FPRINTF(output, "pop  rbp                   ; restore base pointer\n");
    TAB_FPRINTF(output, "; CALL END\n\n");
}

void asm_push_call_params(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> right) { asm_push_call_params(node -> right, ids_table, output, BX_shift); }
        if (node -> left)  { asm_push_call_params(node -> left,  ids_table, output, BX_shift); }
    }
    else if ( (node -> type == OP) && (node -> value.op >= ADD) && (node -> value.op <= POW) )
    {
        asm_translate_Expression(node, ids_table, output, BX_shift);
    }
    else if ( (node -> type == OP) && (node -> value.op == CALL))
    {
        asm_translate_Function_Call(node, ids_table, output, BX_shift);
    }
    else if (node -> type == ID || node -> type == NUM)
    {
        asm_translate_push_node_value(node, ids_table, output);
    }
    else { assert(0); }
}

void asm_translate_Print(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    asm_translate_Expression(node -> left, ids_table, output, BX_shift);
    TAB_FPRINTF(output, "call printf\n\n");
}

void asm_translate_Scan(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n\tcall scanf\n");
    asm_translate_pop_var(node -> left, ids_table, output);
}

#define DEF_OPERATION(enum_name, dump_name, asm_op_name, ...) #asm_op_name,

const char* proc_operations_list[] =
{
    #include "DSL_elden_operations.h"
};
#undef DEF_OPERATION

void asm_translate_Expression(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        // TODO: Check if it is math function???
        if (node -> value.op == CALL)
        {
            asm_translate_Function_Call(node, ids_table, output, BX_shift);
        }
        else
        {

            TAB_FPRINTF(output, "; prepare result of left subtree in stack:\n");
            asm_translate_Expression(node -> left,  ids_table, output, BX_shift);
            TAB_FPRINTF(output, "; prepare result of right subtree in stack:\n");
            asm_translate_Expression(node -> right, ids_table, output, BX_shift);

            //TAB_FPRINTF(output, "pop  rax                   ; get result of right subtree from stack\n");
            //TAB_FPRINTF(output, "pop  rdx                   ; get result of left  subtree from stack\n");
            //TAB_FPRINTF(output, "%s  rax, rdx\n", proc_operations_list[node -> value.op]);      // compute
            //TAB_FPRINTF(output, "push rax                   ; save result in stack\n\n");


            TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; get result of right subtree from stack\n\t"
                                "add   rsp, 8\n");
            TAB_FPRINTF(output, "movsd xmm1, [rsp]          ; get result of left  subtree from stack\n\t"
                                "add   rsp, 8\n");

            switch (node -> value.op) {
                case ADD:   TAB_FPRINTF(output, "addsd xmm0, xmm1\n"); break;
                case SUB:   TAB_FPRINTF(output, "subsd xmm0, xmm1\n"); break;
                case MUL:   TAB_FPRINTF(output, "mulsd xmm0, xmm1\n"); break;
                case DIV:   TAB_FPRINTF(output, "divsd xmm0, xmm1\n"); break;

                default: { fprintf(stderr, "Invalid operation for expression : %s\n", proc_operations_list[node -> value.op]); }
            }
            TAB_FPRINTF(output, "sub rsp, 8\n\t"
                                "movsd [rsp], xmm0          ; save result in stack\n\n");
        }
    }
    else
    {
        asm_translate_push_node_value(node, ids_table, output);
    }
}

void asm_translate_push_node_value(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    // mov  rcx, [bp + %d]
    // push rcx

    // PREPARE VALUE
    if (node -> type == ID)
    {
        if (ids_table[node -> value.id].scope == LOCAL) {
            TAB_FPRINTF(output, "movsd xmm0, [rbp + %d]\n", ids_table[node -> value.id].address);
        }
        else {
            TAB_FPRINTF(output, "movsd xmm0, [rel %.*s]\n", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
        }
    }
    else if (node -> type == NUM)
    {
        TAB_FPRINTF(output, "mov  rax,  __?float64?__(%lg)\n\t"
                            "movq xmm0, rax\n", node -> value.num);
    }

    // PUSH VALUE
    TAB_FPRINTF(output, "sub rsp, 8\n\t"
                        "movsd [rsp], xmm0          ; save result in stack\n\n");
}

void asm_translate_pop_var(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    // pop rcx
    // mov [rbp + %d], rcx

    TAB_FPRINTF(output, "pop rcx\n");
    if (node -> type == ID)
    {
        if (ids_table[node -> value.id].scope == LOCAL) { TAB_FPRINTF(output, "movsd [rbp + %d],  xmm0\n", ids_table[node -> value.id].address); }
        else                                            { TAB_FPRINTF(output, "movsd [rel %.*s], xmm0\n",
                                                                      ids_table[node -> value.id].name_len, ids_table[node -> value.id].name); }
    }
    else
    {
        fprintf(stderr, "ERROR: node of ID type expected\n");
    }
}

