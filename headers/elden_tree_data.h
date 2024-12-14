#ifndef _ELDEN_TREE_DATA_H__
#define _ELDEN_TREE_DATA_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

// THIS MUST BE IN DSL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
enum operation
{
    BOND,
    ASSIGNMENT,
    IF,
    WHILE,

    VAR_DEFINITION,
    FUNCTION_DEFINITION,
    FUNCTION_CALL,
    RETURN,

    ADD,
    SUB,
    MUL,
    DIV,
    SQRT,
    END
};

enum node_type {OP, NUM, ID};

union node_value
{
    size_t op;
    double num;
    size_t id;
};

struct node_t
{
    node_type type;
    node_value value;
    node_t* left;
    node_t* right;
    node_t* parent;
};

#endif
