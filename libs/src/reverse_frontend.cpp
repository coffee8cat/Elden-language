#include "reverse_frontend.h"

static size_t tabs_shift = 0;
static bool is_first_param = true;

#define DEF_TOKEN(name, type, value) name,

const char* operations_names[] =
{
    #include "DSL_elden.h"
    "empty"
};

#undef DEF_TOKEN

#define PRINT_TABS_SHIFT()                                      \
    for (size_t i = 0; i < tabs_shift; i++)                     \
    {                                                           \
        fprintf(output, "\t");                                  \
        printf("\t");                                           \
    }                                                           \

#define PRINT_OUT(...)                                          \
    fprintf(output, __VA_ARGS__);                               \

#define PRINT_GRAMMAR(token_num)                             \
    PRINT_OUT("%s ", operations_names[token_num])            \

 /*
    fprintf(output, );     \
    printf("%s ", operations_names[token_num]);              \
*/

void reverse_frontend(node_t* root, identificator* ids_table)
{
    assert(root);
    assert(ids_table);

    printf("REVERSING FRONTEND\n");

    FILE* fp = get_stream_for_save();
    if (fp == NULL) { fprintf(stderr, "ERROR: Could not open file for reversed_frontend\n"); assert(0); }

    reverse_node(root, ids_table, fp);
    fprintf(fp, "\n$");
    fclose(fp);
}

void reverse_node(node_t* node, identificator* ids_table, FILE* output)
{
    if (!node) { return; }
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        if (node -> value.op == BOND)
        {
            reverse_node(node -> left,  ids_table, output);
            reverse_node(node -> right, ids_table, output);
        }
        else
        {
            switch (node -> value.op)
            {
                case ASSIGNMENT:    { reverse_Assignment  (node, ids_table, output); break; }
                case IF:            { reverse_IF          (node, ids_table, output); break; }
                case WHILE:         { reverse_While       (node, ids_table, output); break; }
                case RTN:           { reverse_Return      (node, ids_table, output); break; }

                case FUNCTION_DEFINITION:   { reverse_Function_Definition (node, ids_table, output); break; }
                case CALL:                  { reverse_Function_Call       (node, ids_table, output); break; }

                case ELEM_IN:  { reverse_Scan  (node, ids_table, output); break;}
                case ELEM_OUT: { reverse_Print (node, ids_table, output); break;}

                case VAR_DEFINITION: { reverse_Var_Definition (node, ids_table, output); break; }

                default: { fprintf(stderr, "ERROR: No such operation\n"); }
            }

            PRINT_OUT(";\n");
        }
    }
    else
    {
        reverse_node_value(node, ids_table, output);
    }
}

void reverse_Assignment(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(ASSIGNMENT_PREFIX);
    reverse_Var(node -> left, ids_table, output);
    PRINT_GRAMMAR(ASSIGNMENT_INFIX);

    reverse_Expression(node -> right, ids_table, output);
}

void reverse_IF(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(node -> left);
    assert(ids_table);
    assert(output);

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(IF_PREFIX);

    //== CONDITION =================================================================================//
    reverse_Expression(node -> left -> left, ids_table, output);

    if (node -> left -> type == OP && node -> left -> value.op == EQUAL)
    {
        PRINT_GRAMMAR(COMP_EQUAL);
    }
    else if (node -> left -> type == OP && node -> left -> value.op == GREATER)
    {
        PRINT_GRAMMAR(COMP_GREATER);
    }
    else { fprintf(stderr, "ERROR: No such operand for comparison: %d", node -> left -> type); }

    reverse_Expression(node -> left -> right, ids_table, output);

    //== BODY =================================================================================//
    PRINT_OUT(", ");
    PRINT_GRAMMAR(IF_POSTFIX);

    PRINT_OUT("\n");
    PRINT_TABS_SHIFT()
    PRINT_OUT("{\n");

    tabs_shift++;
    reverse_node(node -> right, ids_table, output);
    tabs_shift--;

    PRINT_TABS_SHIFT()
    PRINT_OUT("}");
}

void reverse_While(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_OUT("\n");
    PRINT_TABS_SHIFT()
    PRINT_OUT("{\n");

    tabs_shift++;
    reverse_node(node -> right, ids_table, output);
    tabs_shift--;

    PRINT_TABS_SHIFT()
    PRINT_OUT("}\n");

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(WHILE_PREFIX);
    reverse_Expression(node -> left, ids_table, output);
    PRINT_GRAMMAR(WHILE_POSTFIX);
}


void reverse_Return(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(RETURN_PREFIX);
    reverse_Expression(node -> left, ids_table, output);
}

void reverse_Function_Definition(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(FUNCTION_DEFINITION_PREFIX);
    reverse_specification(node -> left, ids_table, output);

    PRINT_OUT("\n");
    PRINT_TABS_SHIFT()
    PRINT_OUT("{\n");

    tabs_shift++;
    reverse_node(node -> right, ids_table, output);
    tabs_shift--;

    PRINT_TABS_SHIFT()
    PRINT_OUT("}");
}

void reverse_Function_Call(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(FUNCTION_CALL_PREFIX);

    reverse_specification(node -> left, ids_table, output);
    PRINT_GRAMMAR(FUNCTION_CALL_POSTFIX);
}

void reverse_specification(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_OUT("%.*s ", ids_table[node -> left -> value.id].name_len, ids_table[node -> left -> value.id].name);

    if (node -> right)
    {
        PRINT_GRAMMAR(SPECIFICATION_INFIX);
        is_first_param = true;
        reverse_params(node -> right, ids_table, output);
    }
}

void reverse_params(node_t* node, identificator* ids_table, FILE* output)
{
    if (node == NULL) { return; }

    assert(ids_table);
    assert(output);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> left)  { reverse_params(node -> left,  ids_table, output); }
        if (node -> right) { reverse_params(node -> right, ids_table, output); }
    }
    else
    {
        reverse_Expression(node, ids_table, output);
    }
    /*if ( (node -> type == OP) && (node -> value.op >= ADD) && (node -> value.op <= POW) )
    {
        reverse_Expression(node, ids_table, output);
    }
    else if ( (node -> type == OP) && (node -> value.op == CALL))
    {
        reverse_Function_Call(node, ids_table, output);
    }
    else if (node -> type == ID || node -> type == NUM)
    {
        if (is_first_param)
        {
            reverse_node_value(node, ids_table);
            return;
        }
        PRINT_OUT(", ");
        reverse_node_value(node, ids_table);
    }
    else { assert(0); }
    */
}

void reverse_Scan(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(SCAN_PREFIX);
    reverse_Expression(node -> left, ids_table, output);
    //PRINT_OUT(", %.*s", ids_table[node -> left-> value.id].name_len, ids_table[node -> left -> value.id].name);
}

void reverse_Print(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(PRINT_PREFIX);
    reverse_Expression(node -> left, ids_table, output);
    //PRINT_OUT("%.*s", ids_table[node -> left-> value.id].name_len, ids_table[node -> left -> value.id].name);
    PRINT_GRAMMAR(PRINT_POSTFIX);
}

void reverse_Expression(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == OP)
    {
        // TODO: Check if it is math function???
        if (node -> value.op == CALL)
        {
            reverse_Function_Call(node, ids_table, output);
        }
        else
        {
            reverse_Expression(node -> left,  ids_table, output);
            printf("%d", node -> left -> type);
            switch(node -> value.op)
            {
                case ADD: { PRINT_GRAMMAR(ADD_LEX); break;}
                case SUB: { PRINT_GRAMMAR(SUB_LEX); break;}
                case MUL: { PRINT_GRAMMAR(MUL_LEX); break;}
                case DIV: { PRINT_GRAMMAR(DIV_LEX); break;}
                case POW: { PRINT_GRAMMAR(POW_LEX); break;}

                default: { fprintf(stderr, "ERROR: Incorrect operation for Expression: %d\n", node -> value.op); }
            }
            reverse_Expression(node -> right, ids_table, output);
        }
    }
    else
    {
        reverse_node_value(node, ids_table, output);
    }
}

void reverse_node_value(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    if (node -> type == ID)
    {
        reverse_Var(node, ids_table, output);
    }
    else
    {
        if ( 0 <= (int)node -> value.num && (int)node -> value.num <= 5)
        {
            PRINT_GRAMMAR((size_t)node -> value.num);
        }
        else { fprintf(stderr, "ERROR: Cannot print number less then 0 or more then 5 in elden language\n"); }
    }
}

void reverse_Var(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(VAR_USAGE_PREFIX);
    PRINT_OUT(" %.*s ", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
}

void reverse_Var_Definition(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_TABS_SHIFT()
    PRINT_GRAMMAR(VAR_USAGE_PREFIX);
    PRINT_OUT("%.*s ", ids_table[node -> left -> value.id].name_len, ids_table[node -> left -> value.id].name);
    PRINT_GRAMMAR(VAR_DEFINITION_POSTFIX);
}
