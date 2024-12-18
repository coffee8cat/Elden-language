#include "reverse_frontend.h"

#define DEF_TOKEN(name, type, value) name,

const char* operations_names[] =
{
    #include "DSL_elden.h"
    "empty"
};

#undef DEF_TOKEN

#define PRINT_GRAMMAR(token_num)                             \
    fprintf(output, "%s ", operations_names[token_num]);     \

void reverse_frontend(node_t* root, identificator* ids_table)
{
    assert(root);
    assert(ids_table);

    FILE* fp = fopen("data\\reversed_elden.txt", "w");
    if (fp == NULL) { fprintf(stderr, "ERROR: Could not open file for reversed_frontend\n"); assert(0); }

    reverse_node(root, ids_table, fp);
    fclose(fp);
}

void reverse_node(node_t* node, identificator* ids_table, FILE* output)
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
                reverse_node(node -> left,  ids_table, output);
                reverse_node(node -> right, ids_table, output);
                break;
            }
            case ASSIGNMENT:    { reverse_Assignment  (node, ids_table, output); break; }
            case IF:            { reverse_IF          (node, ids_table, output); break; }
            case WHILE:         { reverse_While       (node, ids_table, output); break; }
            case RTN:           { reverse_Return      (node, ids_table, output); break; }

            case FUNCTION_DEFINITION:   { reverse_Function_Definition (node, ids_table, output); break; }
            case CALL:                  { reverse_Function_Call       (node, ids_table, output); break; }

            case ELEM_IN:  { reverse_Scan  (node, ids_table, output); break;}
            case ELEM_OUT: { reverse_Print (node, ids_table, output); break;}

            default: { fprintf(stderr, "ERROR: No such operation\n"); }
        }

        fprintf(output, ";\n");
    }
}

void reverse_Assignment(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(ASSIGNMENT_PREFIX);
    reverse_Var(node -> left, ids_table, output);
    PRINT_GRAMMAR(ASSIGNMENT_INFIX);

    reverse_Expression(node -> right, ids_table, output);
}

void reverse_IF(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(IF_PREFIX);
    reverse_Expression(node -> left, ids_table, output);

    fprintf(output, ", ");
    PRINT_GRAMMAR(IF_POSTFIX);

    fprintf(output, "\n{\n");
    reverse_node(node -> right, ids_table, output);
    fprintf(output, "\n}\n");
}

void reverse_While(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "\n{\n");
    reverse_node(node -> right, ids_table, output);
    fprintf(output, "\n}\n");

    PRINT_GRAMMAR(WHILE_PREFIX);
    reverse_Expression(node -> left, ids_table, output);
    PRINT_GRAMMAR(WHILE_POSTFIX);
}


void reverse_Return(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

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

    fprintf(output, "\n{\n");
    reverse_node(node -> right, ids_table, output);
    fprintf(output, "\n}\n\n");
}

void reverse_Function_Call(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(FUNCTION_CALL_PREFIX);

    reverse_specification(node -> left, ids_table, output);
    fprintf(output, "\n{\n");
    reverse_node(node -> right, ids_table, output);
    fprintf(output, "\n}\n\n");
}

void reverse_specification(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    fprintf(output, "%.*s ", ids_table[node -> left -> value.id].name_len, ids_table[node -> left -> value.id].name);
    PRINT_GRAMMAR(SPECIFICATION_INFIX);
    reverse_params(node -> right, ids_table, output);
}

void reverse_params(node_t* node, identificator* ids_table, FILE* output)
{
    if (node == NULL) { return; }

    static bool is_first = true;

    assert(ids_table);
    assert(output);

    if (node -> type == OP && node -> value.op == BOND)
    {
        if (node -> left)  { reverse_params(node -> left,  ids_table, output); }
        if (node -> right) { reverse_params(node -> right, ids_table, output); }
    }
    else if (node -> type == ID)
    {
        if (is_first)
        {
            fprintf(output, "%.*s", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
            is_first = false;
        }
        fprintf(output, ", %.*s", ids_table[node -> value.id].name_len, ids_table[node -> value.id].name);
    }
    else { assert(0); }
}

void reverse_Scan(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(SCAN_PREFIX);
    fprintf(output, ", %.*s", ids_table[node -> left-> value.id].name_len, ids_table[node -> left -> value.id].name);
}

void reverse_Print(node_t* node, identificator* ids_table, FILE* output)
{
    assert(node);
    assert(ids_table);
    assert(output);

    PRINT_GRAMMAR(PRINT_PREFIX);
    fprintf(output, ", %.*s", ids_table[node -> left-> value.id].name_len, ids_table[node -> left -> value.id].name);
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
            PRINT_GRAMMAR(node -> value.op);
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
    fprintf(output, ", %.*s", ids_table[node -> left-> value.id].name_len, ids_table[node -> left -> value.id].name);
}
