
DEF_OPERATION(BOND,                     ";", nop)
DEF_OPERATION(ASSIGNMENT,               "=", nop)

DEF_OPERATION(IF,                       "if",       nop)
DEF_OPERATION(EQUAL,                    "==",       nop)
DEF_OPERATION(GREATER,                  "more",     nop)
DEF_OPERATION(WHILE,                    "while",    nop)
DEF_OPERATION(VAR_DEFINITION,           "var_def",  nop)

DEF_OPERATION(MAIN,                     "main",     nop)
DEF_OPERATION(FUNCTION_DEFINITION,      "def",      nop)
DEF_OPERATION(FUNCTION_SPECIFICATION,   "spec",     nop)
DEF_OPERATION(CALL,                     "call",     nop)
DEF_OPERATION(RTN,                      "return",   nop)

DEF_OPERATION(ELEM_IN,  "scan",     nop)
DEF_OPERATION(ELEM_OUT, "print",    nop)

DEF_OPERATION(ADD,  "+", add)
DEF_OPERATION(SUB,  "-", sub)
DEF_OPERATION(MUL,  "*", mul)
DEF_OPERATION(DIV,  "/", div)
DEF_OPERATION(POW,  "^", pow)

