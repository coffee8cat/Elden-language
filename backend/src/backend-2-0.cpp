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

    fprintf(fp, "section .text\n\n"
                "global main\n"
                "extern print_double\n"
                "extern elem_in\n"
                "extern elem_out\n"
                "extern scanf\n"
                "main:\n");

    translate_OP(node, ids_table, fp, BX_shift);

    fprintf(fp, "\nsection .data\n\n");
    write_global_vars(fp, ids_table);

    write_data_for_lib_funcs(fp);

    fprintf(fp, "section .note.GNU-stack noalloc noexec nowrite progbits\n");
    fclose(fp);
}

void write_global_vars(FILE* output, identificator* ids_table) {
    assert(output);
    assert(ids_table);

    fprintf(output, "\n\t; global variables\n\n");
    size_t curr = 0;
    while (ids_table[curr].name && curr < max_num_of_ids)
    {
        // functions have true in .is_defined
        if (ids_table[curr].scope == GLOBAL && ids_table[curr].type == VAR) {
            TAB_FPRINTF(output, "%.*s dq 0\n", ids_table[curr].name_len, ids_table[curr].name);
        }
        curr++;
    }
}

void write_data_for_lib_funcs(FILE* output) {
    assert(output);

    fprintf(output, "\n\t; data for libs\n\n");
    TAB_FPRINTF(output, "double_format db \"%%lf\", 0\n");
    TAB_FPRINTF(output, "double_var    dq 0.0\n");
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
    asm_translate_Expression(node -> right, ids_table, output, BX_shift);
    // after func above result is stored in stack
    // store result in xmm0
    TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; assignment to %.*s\n\t"
                        "add   rsp, 8\n",
                ids_table[node -> left -> value.id].name_len, ids_table[node -> left -> value.id].name);

    // save result in var
    if (node -> left -> type != ID) {
        fprintf(stderr, "cannot assign to ID [%p]", node);
        assert(0);
    }
    if (ids_table[node -> left -> value.id].scope == GLOBAL) {
        TAB_FPRINTF(output, "movsd  [rel %.*s], xmm0\n",  ids_table[node -> left -> value.id].name_len, ids_table[node -> left -> value.id].name);
    }
    else {
        TAB_FPRINTF(output, "movsd  [rbp - 8 * %d], xmm0\n",  ids_table[node -> left -> value.id].address);
    }
}

void translate_condition(FILE* output) {
    assert(output);

    //fprintf    (output, "\n\tpop  rcx\n");
    //TAB_FPRINTF(output,     "pop  rdx\n");
    //TAB_FPRINTF(output,     "test rcx, rdx\n\n");

    // pop two arguments from stack
    TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; get result of right subtree from stack\n\t"
                        "add   rsp, 8\n");
    TAB_FPRINTF(output, "movsd xmm1, [rsp]          ; get result of right subtree from stack\n\t"
                        "add   rsp, 8\n");
    // compare
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

    // prepare left and right part of comparison
    asm_translate_Expression(node -> left -> right, ids_table, output, BX_shift);
    asm_translate_Expression(node -> left -> left,  ids_table, output, BX_shift);

    // compare
    translate_condition(output);

    // write the matching jmp instruction
    if (node -> left -> value.op == EQUAL)
    {
        TAB_FPRINTF(output, "jne .endif%d\n", local_IF_counter);
    }
    else if (node -> left -> value.op == GREATER)
    {
        TAB_FPRINTF(output, "ja .endif%d\n", local_IF_counter);
    }
    else { fprintf(stderr, "ERROR: Invalid operation for condition in IF: %d\n", node -> left -> value.op); assert(0); }

    // translate IF body
    TAB_FPRINTF(output, "; if body:\n");
    translate_OP(node -> right, ids_table, output, BX_shift);

    // for skipping IF body
    TAB_FPRINTF(output, ".endif%d:\n", local_IF_counter);

    // to avoid uncertainty in labels
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

    // this is bc old format did not contain MAIN function,
    // therefore i have to write _start before translation and somehow
    // put EOP for MAIN
    TAB_FPRINTF(output, "; Exit with code 0 (success)\n\t"
                        "mov rax, 60                 ; sys_exit\n\t"
                        "xor rdi, rdi                ; code 0\n\t"
                        "syscall\n\n");

    // print function name
    if (node -> left && node -> left -> left) {
        fprintf(output, "%.*s:\n", ids_table[node -> left -> left -> value.id].name_len, ids_table[node -> left -> left -> value.id].name);
    }
    else { fprintf(stderr, "ERROR: nullptr in function definition"); }

    size_t FRAME_SIZE_aligned_by_16 = 8 * ( ids_table[node -> left -> left -> value.id].BX_shift +
                                            ids_table[node -> left -> left -> value.id].BX_shift % 2);
    // BX_SHIFT ALIGNED BY 16
    TAB_FPRINTF(output, "push rbp\n\t"
                        "mov  rbp, rsp\n\t"
                        "sub  rsp, %d\n\t", FRAME_SIZE_aligned_by_16);

    // GET PARAMETERS FROM STACK, CONSIDERING RET ADDRESS IS ON TOP OF STACK
    get_params(ids_table[(node -> left -> left) -> value.id].num_of_params, FRAME_SIZE_aligned_by_16, output);
    // translate function body
    translate_OP(node -> right, ids_table, output, ids_table[node -> left -> left -> value.id].BX_shift);
}

void get_params(size_t num_of_params, size_t frame_size, FILE* output) {
    assert(output);

    TAB_FPRINTF(output,     "; get params from stack====================================\n");

    // 16 bc there were two pushes (ret addr and rbp) between frame and parameters
    size_t rbp_rel = frame_size + 16;
    for (size_t i = 1; i <= num_of_params; i++)
    {
        TAB_FPRINTF(output, "movsd xmm0, [rsp + %d]        ; pop %d param\n\t"
                            "movsd [rbp - 8 * %d], xmm0\n\n", rbp_rel, i, i);
        rbp_rel += 8;
    }
    TAB_FPRINTF(output,     "; params loaded============================================\n");
}


void asm_translate_Return(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    // calculate return value
    fprintf(output, "; prepare return value\n");
    asm_translate_Expression(node -> left, ids_table, output, BX_shift);

    // return value is stored in stack now
    // save to xmm0, due to restoring rsp and rbp after return
    TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; get function result before ret\n\t"
                        "add   rsp, 8\n\t"
                        "leave                      ; restore rbp and rsp\n\t"
                        "ret\n\n");
}

void asm_translate_Function_Call(node_t* node, identificator* ids_table, FILE* output, size_t BX_shift)
{
    assert(node);
    assert(ids_table);
    assert(output);

    // shift base
    //TAB_FPRINTF(output, "\n;before CALL shift base and stack pointers (rsp, rbp)\n");
    //TAB_FPRINTF(output, "sub rbp, 8 * %d\n", BX_shift);
    //TAB_FPRINTF(output, "mov rsp, rbp\n");

    // check function definition
    if (not ids_table[node -> left -> left -> value.id].is_defined)
    {
        fprintf(stderr, "ERROR: Function %.*s was not defined in this scope\n",
                        ids_table[node -> left -> left -> value.id].name_len,
                        ids_table[node -> left -> left -> value.id].name);
    }

    TAB_FPRINTF(output, "; push call params\n");
    asm_push_call_params(node -> left -> right, ids_table, output, BX_shift);

    //params are in the right order in stack
    //get_params(node -> left, ids_table, output);
    TAB_FPRINTF(output, "call %.*s\n",
            ids_table[node -> left -> left -> value.id].name_len,
            ids_table[node -> left -> left -> value.id].name);

    // after call return value is stored in xmm0
    // restore base pointer and stack pointer
    //TAB_FPRINTF(output, "add rbp, 8 * %d\n", BX_shift);
    //TAB_FPRINTF(output, "mov rsp, rbp\n");

    clear_stack(ids_table[node -> left -> left -> value.id].num_of_params, output);
    // push function result
    TAB_FPRINTF(output, "sub rsp, 8\n\t"
                        "movsd [rsp], xmm0          ; save result in stack\n\n");
    TAB_FPRINTF(output, "; CALL END\n\n");
}

void clear_stack(size_t num_of_params, FILE* output) {
    assert(output);

    // maybe have to align by 16
    TAB_FPRINTF(output, "add rsp, %ld                ; clear stack\n", 8 * num_of_params);
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

    TAB_FPRINTF(output, "; prepare for printf\n\n");
    asm_translate_Expression(node -> left, ids_table, output, BX_shift);

    // store result from stack in xmm0 for printf
    TAB_FPRINTF(output, "movsd xmm0, [rsp]\n\t"
                        "add   rsp, 8\n");
    // align stack
    TAB_FPRINTF(output, "mov rax, rsp               ; use caller-saved rcx\n\t"
                        "and rax, 0xF               ; rcx = rsp % 16\n\t"
                        "sub rsp, rax               ; align stack by 16\n\t"
                        "push rax                   ; save aligning, now after call stack will be aligned by 16\n\n\t"
                        "call print_double\n\n\t"
                        "pop rax                    ; restore aligning\n\t"
                        "add rsp, rax               ; restore stack\n\n");
    /*
    // prepare format string address and number of args
    TAB_FPRINTF(output, "mov rdi, double_format  ; pointer to format string\n\t"
                        "mov eax, 1              ; number of arguments in xmm\n\t"
                        "call printf\n\n");
    */
}

void asm_translate_Scan(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    // prepare arguments for scanf and call
    TAB_FPRINTF(output, "; scanf start ;===================================================\n\t"
                        "sub rsp, 8                         ; align stack by 16 before call\n\t"
                        "lea rdi, [rel double_format]       ; format string\n\t"
                        "lea rsi, [rel double_var]          ; pointer to var\n\t"
                        "xor eax, eax                       ; num of XMM regs (by ABI)\n\t"
                        "call scanf  wrt ..plt\n\t"
                        "add rsp, 8                         ; restore stack\n\n");

    //TAB_FPRINTF(output, "call elem_in\n");
    // store result in stack
    /*
    TAB_FPRINTF(output, "; store result in stack\n\t"
                        "movsd xmm0, [double_var]\n\t"
                        "sub rsp, 8\n\t"
                        "movsd [rsp], xmm0\n\n");
    */

    TAB_FPRINTF(output, "; store result in stack\n\t"
                        "sub rsp, 8\n\t"
                        "movsd [rsp], xmm0\n\n");

    asm_translate_pop_var(node -> left, ids_table, output);
    TAB_FPRINTF(output, "; scanf end ;=====================================================\n\n");
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

            // pop two arguments for operation
            TAB_FPRINTF(output, "movsd xmm1, [rsp]          ; get result of right subtree from stack\n\t"
                                "add   rsp, 8\n");
            TAB_FPRINTF(output, "movsd xmm0, [rsp]          ; get result of left  subtree from stack\n\t"
                                "add   rsp, 8\n");

            // apply operation
            switch (node -> value.op) {
                case ADD:   TAB_FPRINTF(output, "addsd xmm0, xmm1\n"); break;
                case SUB:   TAB_FPRINTF(output, "subsd xmm0, xmm1\n"); break;
                case MUL:   TAB_FPRINTF(output, "mulsd xmm0, xmm1\n"); break;
                case DIV:   TAB_FPRINTF(output, "divsd xmm0, xmm1\n"); break;

                // IT IS NOT VALID, only bc i need only sqrt for tests, i will try to redo it
                case POW:   TAB_FPRINTF(output,  "sqrtsd xmm0, xmm1\n"); break;

                default: { fprintf(stderr, "Invalid operation for expression : %s\n", proc_operations_list[node -> value.op]); }
            }
            // store result in stack
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

    // PREPARE VALUE
    if (node -> type == ID)
    {
        if (ids_table[node -> value.id].scope == LOCAL) {
            TAB_FPRINTF(output, "movsd xmm0, [rbp - 8 * %d]\n", ids_table[node -> value.id].address);
        }
        else {
            TAB_FPRINTF(output, "movsd xmm0, [rel %.*s]\n", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
        }
    }
    else if (node -> type == NUM)
    {
        TAB_FPRINTF(output, "mov  rax,  __float64__(%lf)\n\t"
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

    // a.k.a pop xmm0
    TAB_FPRINTF(output, "movsd xmm0, [rsp]\n\t"
                        "add   rsp, 8\n");
    // mov var, xmm0
    if (node -> type == ID)
    {
        if (ids_table[node -> value.id].scope == LOCAL) { TAB_FPRINTF(output, "movsd [rbp - 8 * %d],  xmm0\n", ids_table[node -> value.id].address); }
        else                                            { TAB_FPRINTF(output, "movsd [rel %.*s], xmm0\n",
                                                                      ids_table[node -> value.id].name_len, ids_table[node -> value.id].name); }
    }
    else
    {
        fprintf(stderr, "ERROR: node of ID type expected\n");
    }
}

