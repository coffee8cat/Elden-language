
#define _BOND(a, b)  new_node(OP, OP_VALUE(BOND),  a, b)
#define _IF(a, b)    new_node(OP, OP_VALUE(IF),    a, b)
#define _WHILE(a, b) new_node(OP, OP_VALUE(WHILE), a, b)

#define _ASSIGNMENT(a, b)               new_node(OP, OP_VALUE(ASSIGNMENT),              a, b)
#define _VAR_DEFINITION(a)              new_node(OP, OP_VALUE(VAR_DEFINITION),          a, NULL)
#define _FUNCTION_DEFINITION(a, b)      new_node(OP, OP_VALUE(FUNCTION_DEFINITION),     a, b)
#define _FUNCTION_SPECIFICATION(a, b)   new_node(OP, OP_VALUE(FUNCTION_SPECIFICATION),  a, b)
#define _FUNCTION_CALL(a, b)            new_node(OP, OP_VALUE(FUNCTION_CALL),           a, b)

#define _RETURN(a) new_node(OP, OP_VALUE(RETURN), a, NULL)
#define _PRINT(a)  new_node(OP, OP_VALUE(PRINT),  a, NULL)
#define _SCAN(a)   new_node(OP, OP_VALUE(SCAN),   a, NULL)

#define _ADD(a, b)  new_node(OP, OP_VALUE(ADD),  a, b)
#define _SUB(a, b)  new_node(OP, OP_VALUE(SUB),  a, b)
#define _MUL(a, b)  new_node(OP, OP_VALUE(MUL),  a, b)
#define _DIV(a, b)  new_node(OP, OP_VALUE(DIV),  a, b)
#define _SQRT(a, b) new_node(OP, OP_VALUE(SQRT), a, b)

#define OP_VALUE(oper)  node_value {.op = (oper)}
#define NUM_VALUE(n)    node_value {.num = (n)}
#define ID_VALUE(x)     node_value {.id = (x)}

#define _NUM(n)  new_node(NUM,  NUM_VALUE(n), NULL, NULL)
#define _ID(x)   new_node(ID,   ID_VALUE(x),  NULL, NULL)