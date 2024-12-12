
//== GRAMMAR ===========================================================================//
DEF_OPERATION_TOKEN(";", G0 =';')
DEF_OPERATION_TOKEN(",", G1 =',')
DEF_OPERATION_TOKEN("{", G2 ='{')
DEF_OPERATION_TOKEN("}", G3 ='}')
DEF_OPERATION_TOKEN("(", G4 ='(')
DEF_OPERATION_TOKEN(")", G5 =')')

//== OPERATIONS ========================================================================//
DEF_OPERATION_TOKEN("And when the stars will fall along with",    IF_PREFIX)
DEF_OPERATION_TOKEN("bless them with",                            IF_POSTFIX)
DEF_OPERATION_TOKEN("As I awaited",       WHILE_PREFIX)
DEF_OPERATION_TOKEN("return",             WHILE_POSTFIX)
DEF_OPERATION_TOKEN("is now",             ASSIGNMENT)
DEF_OPERATION_TOKEN("forged",             VAR_DEFINITION)
DEF_OPERATION_TOKEN("the ring of ",       VAR_USAGE)

//== FUNCTIONS =========================================================================//
DEF_OPERATION_TOKEN("The grace of", FUNCTION_DEFINITION_PREFIX)
DEF_OPERATION_TOKEN("upon",         FUNCTION_DEFINITION_POSTFIX)

DEF_OPERATION_TOKEN("Bless thy tarnished with", RETURN)

DEF_OPERATION_TOKEN("Let thy grace of", FUNCTION_CALL_PREFIX)
DEF_OPERATION_TOKEN("shine again upon", FUNCTION_CALL_POSTFIX)

//== MATH OPERATIONS ===================================================================//
DEF_OPERATION_TOKEN("smelted with",   ADD)
DEF_OPERATION_TOKEN("shadowed by",    SUB)
DEF_OPERATION_TOKEN("enchanted with", MUL)
DEF_OPERATION_TOKEN("fractured by",   DIV)

//== NUMBERS ===========================================================================//
DEF_NUMBER_TOKEN("rune of death",      0)
DEF_NUMBER_TOKEN("Starling amber",     1)
DEF_NUMBER_TOKEN("Lost ashes of war",  2)
DEF_NUMBER_TOKEN("Messmer's splinter", 3)
DEF_NUMBER_TOKEN("pure golden needle", 4)
DEF_NUMBER_TOKEN("Michella's needle",  5)
