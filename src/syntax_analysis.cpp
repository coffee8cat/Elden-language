#include "syntax_analysis.h"

#define GRAMMAR_CHECK(word)                                                                         \
    if (! (lexeme_array[*curr].type == LEX_OP && lexeme_array[*curr].value == word)) {assert(0);}   \
    (*curr)++                                                                                       \

node_t* get_General (lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);

    // this is bullshit for now
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
    if ((node = get_Assignment(lexeme_array, curr, html_stream))                != NULL) { continue; }
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

    if (! (var = get_Var(lexeme_array, curr, html_stream))) { assert(0); }
    GRAMMAR_CHECK(ASSIGNMENT_GRAMMAR);
    if (! (expression = get_Expression(lexeme_array, curr, html_stream))) { assert(0); }

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

    GRAMMAR_CHECK(IF_PREFIX);

    if (condition = get_Expression(lexeme_array, curr, html_stream)) { assert(0); }
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

    GRAMMAR_CHECK('{');

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

node_t* get_Operation(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);
}
node_t* get_Operation(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);
}
node_t* get_Operation(lexeme_t* lexeme_array, size_t* curr, FILE* html_stream)
{
    assert(lexeme_array);
    assert(curr);
    assert(html_stream);
}
