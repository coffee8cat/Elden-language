
#define _BOND(a, b)     new_node(OP, OP_VALUE(BOND),    a, b)
#define _IF(a, b)       new_node(OP, OP_VALUE(IF),      a, b)
#define _EQUAL(a, b)    new_node(OP, OP_VALUE(EQUAL),   a, b)
#define _GREATER(a, b)  new_node(OP, OP_VALUE(GREATER), a, b)
#define _WHILE(a, b)    new_node(OP, OP_VALUE(WHILE),   a, b)

#define _ASSIGNMENT(a, b)               new_node(OP, OP_VALUE(ASSIGNMENT),              a, b)
#define _VAR_DEFINITION(a)              new_node(OP, OP_VALUE(VAR_DEFINITION),          a, NULL)
#define _MAIN(a, b)                     new_node(OP, OP_VALUE(MAIN),                    a, b)
#define _FUNCTION_DEFINITION(a, b)      new_node(OP, OP_VALUE(FUNCTION_DEFINITION),     a, b)
#define _FUNCTION_SPECIFICATION(a, b)   new_node(OP, OP_VALUE(FUNCTION_SPECIFICATION),  a, b)
#define _FUNCTION_CALL(a, b)            new_node(OP, OP_VALUE(CALL),                    a, b)

#define _RETURN(a) new_node(OP, OP_VALUE(RTN),       a, NULL)
#define _PRINT(a)  new_node(OP, OP_VALUE(ELEM_OUT),  a, NULL)
#define _SCAN(a)   new_node(OP, OP_VALUE(ELEM_IN),   a, NULL)

#define _ADD(a, b)  new_node(OP, OP_VALUE(ADD), a, b)
#define _SUB(a, b)  new_node(OP, OP_VALUE(SUB), a, b)
#define _MUL(a, b)  new_node(OP, OP_VALUE(MUL), a, b)
#define _DIV(a, b)  new_node(OP, OP_VALUE(DIV), a, b)
#define _POW(a, b)  new_node(OP, OP_VALUE(POW), a, b)

#define OP_VALUE(oper)  node_value {.op = (oper)}
#define NUM_VALUE(n)    node_value {.num = (n)}
#define ID_VALUE(x)     node_value {.id = (x)}

#define _NUM(n)  new_node(NUM,  NUM_VALUE(n), NULL, NULL)
#define _ID(x)   new_node(ID,   ID_VALUE(x),  NULL, NULL)
