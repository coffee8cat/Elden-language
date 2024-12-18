#include "syntax_analysis.h"
#include "DSL_comp_tree.h"

#define SYNTAX_ERROR(expected)                                                                  \
    fprintf(stderr, "\n\n!!! SYNTAX ERROR !!!\n"                                                \
                    "In %s:%d in %s on position %d expected <%s>, but <%d(%c)> found\n",        \
                    __FILE__, __LINE__, __func__, *curr, #expected,                             \
                    lexeme_array[*curr].value, lexeme_array[*curr].value);                      \
    assert(0)                                                                                   \

// How the hell do i understand what is fucked by a NUMBER of expected lexeme???


#define CHECK_WORD(word) (lexeme_array[*curr].type == OP && lexeme_array[*curr].value == (word))

#define GRAMMAR_CHECK(word)                                 \
    if (not CHECK_WORD(word)) {SYNTAX_ERROR(word);}         \
    (*curr)++                                               \

#define SOFT_GRAMMAR_CHECK(word)                 \
    if (not CHECK_WORD(word)) { return NULL; }   \
    (*curr)++                                    \


node_t* get_General (lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = NULL;
    if(not (node = get_Operation(lexeme_array, ids_table, curr, html_stream))) { SYNTAX_ERROR(Operation); }
    GRAMMAR_CHECK(';');

    node_t* temp = NULL;
    while ((temp = get_Operation(lexeme_array, ids_table, curr, html_stream)) != NULL)
    {
        node = _BOND(node, temp);
        GRAMMAR_CHECK(';');
    }

    return node;
}

node_t* get_Operation(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = NULL;
    if      ((node = get_Assignment         (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_IF                 (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_While              (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_Var_Definition     (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_Function_Definition(lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_Function_Call      (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_Return             (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_Scan               (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    else if ((node = get_Print              (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }

    return NULL;
}

//<Assignment> ::= "And now" <space> <Var> <space> "is" <space> <Expression>
node_t* get_Assignment(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* var  = NULL;
    node_t* node = NULL;

    SOFT_GRAMMAR_CHECK(ASSIGNMENT_PREFIX);
    if ((var = get_Var(lexeme_array, ids_table, curr, html_stream)) == NULL) { return NULL; }
    GRAMMAR_CHECK(ASSIGNMENT_INFIX);
    if      ((node = get_Expression(lexeme_array, ids_table, curr, html_stream))    != NULL) { return _ASSIGNMENT(var, node); }

    SYNTAX_ERROR(Expression or function call);
    return NULL;
}

//<If> ::= "And when the stars will fall along with" <Expression> <Comparison Operator> <Expression>
//"," "bless them with" "{" (<Operation> ";")+ "}"
node_t* get_IF(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* left_cond_part  = NULL;
    node_t* right_cond_part = NULL;
    node_t* condition = NULL;
    node_t* action    = NULL;

    SOFT_GRAMMAR_CHECK(IF_PREFIX);

    if ((left_cond_part = get_Expression(lexeme_array, ids_table, curr, html_stream)) == NULL) { SYNTAX_ERROR(Expression); }
    if (CHECK_WORD(COMP_EQUAL))
    {
        (*curr)++;
        if ((right_cond_part = get_Expression(lexeme_array, ids_table, curr, html_stream)) == NULL) { SYNTAX_ERROR(Expression); }
        condition = _EQUAL(left_cond_part, right_cond_part);
    }
    else if (CHECK_WORD(COMP_GREATER))
    {
        (*curr)++;
        if ((right_cond_part = get_Expression(lexeme_array, ids_table, curr, html_stream)) == NULL) { SYNTAX_ERROR(Expression); }
        condition = _GREATER(left_cond_part, right_cond_part);
    }
    else { SYNTAX_ERROR(comparison operator: (equal) or (greater)); }

    GRAMMAR_CHECK(',');
    GRAMMAR_CHECK(IF_POSTFIX);
    GRAMMAR_CHECK('{');

    action = get_Operation(lexeme_array, ids_table, curr, html_stream);
    GRAMMAR_CHECK(';');

    node_t* temp = NULL;
    while ((temp = get_Operation(lexeme_array, ids_table, curr, html_stream)) != NULL)
    {
        action = _BOND(action, temp);
        GRAMMAR_CHECK(';');
    }

    GRAMMAR_CHECK('}');

    return _IF(condition, action);
}

//<While> ::= "{" (<Operation> ";")+ "}" "As I awaited" <Expression> "return"
node_t* get_While(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* condition = NULL;
    node_t* action    = NULL;

    SOFT_GRAMMAR_CHECK('{');

    if ((action = get_Operation(lexeme_array, ids_table, curr, html_stream)) == NULL) { SYNTAX_ERROR(Operation); }
    GRAMMAR_CHECK(';');

    node_t* temp = NULL;
    while ((temp = get_Operation(lexeme_array, ids_table, curr, html_stream)) != NULL)
    {
        action = _BOND(action, temp);
        GRAMMAR_CHECK(';');
    }

    GRAMMAR_CHECK('}');
    GRAMMAR_CHECK(WHILE_PREFIX);
    GRAMMAR_CHECK(WHILE_POSTFIX);

    return _WHILE(condition, action);
}

node_t* get_Var_Definition(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* var = NULL;

    if ((var = get_Var(lexeme_array, ids_table, curr, html_stream)) == NULL) { return NULL; }
    GRAMMAR_CHECK(VAR_DEFINITION_POSTFIX);

    return _VAR_DEFINITION(var);
}

//<Function_definition> ::= "The grace of" <space> <ID> <space> "upon the" <Var> ("," <Var>)* "{" <space> (<Operation> ";")+ <space> "}"
node_t* get_Function_Definition(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* id      = NULL;
    node_t* params  = NULL;
    node_t* temp    = NULL;
    SOFT_GRAMMAR_CHECK(FUNCTION_DEFINITION_PREFIX);

    if (not (lexeme_array[*curr].type == ID)) { SYNTAX_ERROR(ID); }
    id = _ID(lexeme_array[*curr].value);
    (*curr)++;

    if (CHECK_WORD(SPECIFICATION_INFIX))
    {
        (*curr)++;
        params = get_Expression(lexeme_array, ids_table, curr, html_stream);
        temp = NULL;
        params = _BOND(params, NULL);
        while (true)
        {
            if (CHECK_WORD(','))
            {
                (*curr)++;
                temp = get_Expression(lexeme_array, ids_table, curr, html_stream);
                if (temp == NULL) { SYNTAX_ERROR(Expression); }
            }
            else { break; }
            params = _BOND(temp, params);
        }
    }
    node_t* specification = _FUNCTION_SPECIFICATION(id, params);

    tree_dump(specification, ids_table, html_stream, specification);

    GRAMMAR_CHECK('{');
    node_t* body = get_Operation(lexeme_array, ids_table, curr, html_stream);
    GRAMMAR_CHECK(';');

    temp = NULL;
    while ((temp = get_Operation(lexeme_array, ids_table, curr, html_stream)) != NULL)
    {
        body = _BOND(body, temp);
        GRAMMAR_CHECK(';');
    }
    GRAMMAR_CHECK('}');

    return _FUNCTION_DEFINITION( specification, body);
}

//<Function_call> ::= "Let thy grace of" <space> <Expression> <space> "shine again upon" <space> <Var> ("," <space> <Var> )*
node_t* get_Function_Call(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    SOFT_GRAMMAR_CHECK(FUNCTION_CALL_PREFIX);

    node_t* id = NULL;
    node_t* params = NULL;
    node_t* temp = NULL;

    if (not (lexeme_array[*curr].type == ID)) { SYNTAX_ERROR(ID); }
    id = _ID(lexeme_array[*curr].value);
    (*curr)++;

    if (CHECK_WORD(SPECIFICATION_INFIX))
    {
        (*curr)++;
        params = get_Expression(lexeme_array, ids_table, curr, html_stream);
        temp = NULL;
        params = _BOND(params, NULL);
        while (true)
        {
            if (CHECK_WORD(','))
            {
                (*curr)++;
                temp = get_Expression(lexeme_array, ids_table, curr, html_stream);
                if (temp == NULL) { SYNTAX_ERROR(Expression); }
            }
            else { break; }
            params = _BOND(temp, params);
        }
    }
    return _FUNCTION_CALL( _FUNCTION_SPECIFICATION(id, params), NULL);
}

//<Return> ::= "Bless thy tarnished with" <space> <Expression>
node_t* get_Return(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    SOFT_GRAMMAR_CHECK(RETURN_PREFIX);
    node_t* return_expression = get_Expression(lexeme_array, ids_table, curr, html_stream);

    return _RETURN(return_expression);
}

//<Scan>  ::= "In the Lands Between we found" <Var>
node_t* get_Scan(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    SOFT_GRAMMAR_CHECK(SCAN_PREFIX);

    node_t* var = get_Var(lexeme_array, ids_table, curr, html_stream);
    if (var == NULL) { SYNTAX_ERROR(var); }

    return _SCAN(var);
}

//<Print> ::= "In the age of Duskborn" <space> ( <Var> | <Expression> ) <space> "will thunder in the darkest night"
node_t* get_Print(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    SOFT_GRAMMAR_CHECK(PRINT_PREFIX);

    node_t* var = get_Var(lexeme_array, ids_table, curr, html_stream);
    if (var == NULL) { SYNTAX_ERROR(var); }

    GRAMMAR_CHECK(PRINT_POSTFIX);
    return _PRINT(var);
}

//<Var> ::= "the ring of " <ID>
node_t* get_Var(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    SOFT_GRAMMAR_CHECK(VAR_USAGE_PREFIX);

    if (not (lexeme_array[*curr].type == ID)) { SYNTAX_ERROR(ID); }
    return _ID(lexeme_array[(*curr)++].value);
}

//<Expression> ::= <space> <T> <space> (("smelted with" | "shadowed by") <space> <T>)*
node_t* get_Expression(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = get_T(lexeme_array, ids_table, curr, html_stream);

    while(lexeme_array[*curr].value == ADD_LEX || lexeme_array[*curr].value == SUB_LEX)
    {
        int op = lexeme_array[*curr].value;
        (*curr)++;
        node_t* node2 = get_T(lexeme_array, ids_table, curr, html_stream);

        if (op == ADD_LEX)  { node = _ADD(node, node2); }
        else                { node = _SUB(node, node2); }
    }

    return node;
}

//<T> ::= <POWER> <space> (("enchanted with" | "fractured by") <space> <POWER>)*
node_t* get_T(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = get_POW(lexeme_array, ids_table, curr, html_stream);
    while(lexeme_array[*curr].value == MUL_LEX || lexeme_array[*curr].value == DIV_LEX)
    {
        char op = (char)lexeme_array[*curr].value;
        (*curr)++;
        node_t* node2 = get_POW(lexeme_array, ids_table, curr, html_stream);

        if (op == MUL_LEX)  { node = _MUL(node, node2); }
        else                { node = _DIV(node, node2); }
    }
    return node;
}

//<POWER> ::= <Primary> <space> (("enchanted with" | "fractured by") <space> <Primary>)*
node_t* get_POW(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = get_Primary(lexeme_array, ids_table, curr, html_stream);
    while(lexeme_array[*curr].value == POW_LEX)
    {
        (*curr)++;
        node_t* node2 = get_Primary(lexeme_array, ids_table, curr, html_stream);
        node = _POW(node, node2);
    }
    return node;
}

//<Primary> ::= "(" <Expression> ")" | <Number> | <Var> | <Call>
node_t* get_Primary(lexeme_t* lexeme_array, identificator* ids_table, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    node_t* node = NULL;
    if ((node = get_Var           (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }
    if ((node = get_Function_Call (lexeme_array, ids_table, curr, html_stream)) != NULL) { return node; }

    if (lexeme_array[*curr].type == NUM) { return _NUM(lexeme_array[(*curr)++].value); }

    if ((char)lexeme_array[*curr].value == '(')
    {
        (*curr)++;
        node = get_Expression(lexeme_array, ids_table, curr, html_stream);

        GRAMMAR_CHECK(')');
        return node;
    }

    return NULL;
}

#undef SYNTAX_ERROR
#undef CHECK_WORD
#undef GRAMMAR_CHECK
#undef SOFT_GRAMMAR_CHECK

