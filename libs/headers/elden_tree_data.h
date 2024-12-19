#ifndef _ELDEN_TREE_DATA_H__
#define _ELDEN_TREE_DATA_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#define DEF_TOKEN(name, type, value) value,

enum lexical_operations
{
    #include "DSL_elden.h"
    UNKNOWN
};

#undef DEF_TOKEN

#define DEF_OPERATION(enum_name, dump_name) enum_name,

enum operation
{
    #include "DSL_elden_operations.h"
    END
};
#undef DEF_OPERATION

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
