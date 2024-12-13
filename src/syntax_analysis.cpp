#include "syntax_analysis.h"

#define SYNTAX_ERROR(expected)                                                                  \
    fprintf(stderr, "\n\n!!! SYNTAX ERROR !!!\n"                                                \
                    "In %s:%d in %s on position %d expected <%s>, but <%d(%c)> found\n",        \
                    __FILE__, __LINE__, __func__, *curr, #expected,                             \
                    lexeme_array[*curr].value, lexeme_array[*curr].value);                      \
    assert(0)                                                                                   \

// How the hell do i understand what is fucked with a NUMBER of expected lexeme???


#define CHECK_WORD(word) (lexeme_array[*curr].type == LEX_OP && lexeme_array[*curr].value == (word))

#define GRAMMAR_CHECK(word)                                 \
    if (not CHECK_WORD(word)) {SYNTAX_ERROR(word);}         \
    (*curr)++                                               \

#define SOFT_GRAMMAR_CHECK(word)                 \
    if (not CHECK_WORD(word)) { return NULL; }   \
    (*curr)++                                    \


node_t* get_General (lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = get_Operation(lexeme_array, curr, html_stream);
    node_t* temp = NULL;
    while (temp = get_Operation(lexeme_array, curr, html_stream))
    {
        node = _BOND(node, temp);
        CHECK_GRAMMAR(';');
    }

    return node;
}

node_t* get_Operation(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = NULL;
    if      ((node = get_Assignment(lexeme_array, curr, html_stream))           != NULL) { continue; }
    else if ((node = get_If(lexeme_array, curr, html_stream))                   != NULL) { continue; }
    else if ((node = get_While(lexeme_array, curr, html_stream))                != NULL) { continue; }
    else if ((node = get_Var_Definition(lexeme_array, curr, html_stream))       != NULL) { continue; }
    else if ((node = get_Function_Definition(lexeme_array, curr, html_stream))  != NULL) { continue; }
    else if ((node = get_Function_Call(lexeme_array, curr, html_stream))        != NULL) { continue; }
    else if ((node = get_Return(lexeme_array, curr, html_stream))               != NULL) { continue; }

    else { return NULL; }
    GRAMMAR_CHECK(';');

    return node;
}

node_t* get_Assignment(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* var  = NULL;
    node_t* expression = NULL;

    if (var = get_Var(lexeme_array, curr, html_stream) == NULL) { return NULL; }
    GRAMMAR_CHECK(ASSIGNMENT_GRAMMAR);
    if (! (expression = get_Expression(lexeme_array, curr, html_stream))) { SYNTAX_ERROR(Expression); }

    return _ASSIGNMENT(var, expression);
}

//<If> ::= "And when the stars will fall along with" <Expression> "," "bless them with" "{" (<Operation> ";")+ "}"
node_t* get_IF(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* condition = NULL;
    node_t* action    = NULL;

    SOFT_GRAMMAR_CHECK(IF_PREFIX);

    if (condition = get_Expression(lexeme_array, curr, html_stream)) { SYNTAX_ERROR(Expression); }
    GRAMMAR_CHECK(',');
    GRAMMAR_CHECK(IF_POSTFIX);
    GRAMMAR_CHECK('{');

    action = get_Operation(lexeme_array, curr, html_stream);
    node_t* temp = NULL;
    while (temp = get_Operation(lexeme_array, curr, html_stream))
    {
        action = _BOND(action, temp);
        CHECK_GRAMMAR(';');
    }

    GRAMMAR_CHECK('}');

    return _IF(condition, action);
}

//<While> ::= "{" (<Operation> ";")+ "}" "As I awaited" <Expression> "return"
node_t* get_While(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* condition = NULL;

    SOFT_GRAMMAR_CHECK('{');

    node_t* action = get_Operation(lexeme_array, curr, html_stream);
    node_t* temp = NULL;
    while (temp = get_Operation(lexeme_array, curr, html_stream))
    {
        action = _BOND(action, temp);
        CHECK_GRAMMAR(';');
    }

    GRAMMAR_CHECK('}');
    GRAMMAR_CHECK(WHILE_PREFIX);
    GRAMMAR_CHECK(WHILE_POSTFIX);

    return _WHILE(condition, action);
}

node_t* get_Var_Definition(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);
    node_t* var = NULL;

    if ((var = get_Var(lexeme_array, curr, html_stream)) == NULL) { return NULL; }
    GRAMMAR_CHECK(VAR_DEFINITION_POSTFIX);

    return var;
}

//<Function_definition> ::= "The grace of" <space> <ID> <space> "upon the" <Var> ("," <Var>)* "{" <space> (<Operation> ";")+ <space> "}"
node_t* get_Function_Definition(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* id = NULL;
    SOFT_GRAMMAR_CHECK(FUNCTION_DEFINITION_PREFIX);

    if (not (lexeme_array[*curr].type == ID)) { SYNTAX_ERROR(ID); }
    id = _ID(lexeme_array[*curr].value);

    GRAMMAR_CHECK(FUNCTION_DEFINITION_POSTFIX);

    node_t* params  = get_Var(lexeme_array, curr, html_stream);
    node_t* temp = params -> right;
    while (CHECK_WORD(',') && (temp = get_Var(lexeme_array, curr, html_stream)))
    {
        temp = temp -> right;
    }

    return _FUNCTION_DEFINITION(id, params);
}

//<Function_call> ::= "Let thy grace of" <space> <ID> <space> "shine again upon" <space> <Var> ("," <space> <Var> )*
node_t* get_Function_Call(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* id = NULL;
    SOFT_GRAMMAR_CHECK(FUNCTION_CALL_PREFIX);

    if (not (lexeme_array[*curr].type == ID)) { SYNTAX_ERROR(ID); }
    id = _ID(lexeme_array[*curr].value);

    GRAMMAR_CHECK(FUNCTION_CALL_POSTFIX);

    node_t* params  = get_Var(lexeme_array, curr, html_stream);
    node_t* temp = params -> right;
    while (CHECK_WORD(',') && (temp = get_Var(lexeme_array, curr, html_stream)))
    {
        temp = temp -> right;
    }

    return _FUNCTION_CALL(id, params);
}

//<Return> ::= "Bless thy tarnished with" <space> <Expression>
node_t* get_Return(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    SOFT_GRAMMAR_CHECK(RETURN_PREFIX);
    node_t* return_expression = get_Expression(lexeme_array, curr, html_stream);

    return _RETURN(return_expression);
}

//<Expression> ::= <space> <T> <space> (("smelted with" | "shadowed by") <space> <T>)*
node_t* get_Expression(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = get_T(lexeme_array, curr, html_stream);

    while(lexeme_array[*curr].value == ADD || lexeme_array[*curr].value == SUB)
    {
        int op = lexeme_array[*curr].value;
        (*curr)++;
        node_t* node2 = get_T(lexeme_array, curr, html_stream);

        if (op == ADD) { node = _ADD(node, node2); }
        else           { node = _SUB(node, node2); }
    }

    return node;
}

//<T> ::= <Primary> <space> (("enchanted with" | "fractured by") <space> <Primary>)*
node_t* get_T(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = get_Primary(lexeme_array, curr, html_stream);
    while(lexeme_array[*curr].value == MUL || lexeme_array[*curr].value == DIV)
    {
        char op = (char)lexeme_array[*curr].value;
        (*curr)++;
        node_t* node2 = get_Primary(lexeme_array, curr, html_stream);

        if (op == MUL) { node = _MUL(node, node2); }
        else           { node = _DIV(node, node2); }
    }
    return node;
}

//<Primary> ::= "(" <Expression> ")" | <Number> | <Var>
node_t* get_Primary(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    switch(lexeme_array[*curr].type)
    {
        case LEX_OP:
        {
            if ((char)lexeme_array[*curr].value == '(')
            {
                (*curr)++;
                GRAMMAR_DEBUG_PRINT();
                node_t* node = GetE(lexeme_array, curr, html_stream);
                GRAMMAR_DEBUG_PRINT();
                if ((char)lexeme_array[*curr].value != ')') { assert(0); }
                (*curr)++;
                GRAMMAR_DEBUG_PRINT();
                return node;
            }
            else
            {
                SYNTAX_ERROR((Expression));
                /*
                printf("---ADDING OPERATION ---\n");
                operations operation = (operations)lexeme_array[*curr].value;
                (*curr)++;
                return new_node(OP, OP_VALUE(operation), NULL, GetE(lexeme_array, curr, html_stream));
                */
            }
        }
        case ID:      { return _VAR(lexeme_array[(*curr)++].value); }
        case LEX_NUM: { return _NUM(lexeme_array[(*curr)++].value); }

        default: SYNTAX_ERROR(LEX_OP | ID | LEX_NUM);
                 //fprintf(stderr, "ERROR: Invalid lexeme type: %d\n", lexeme_array[*curr].type);
                 break;
    }
}
