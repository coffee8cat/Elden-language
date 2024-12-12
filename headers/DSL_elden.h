DEF_GRAMMAR_TOKEN(";", ';')
DEF_GRAMMAR_TOKEN(",", ',')
DEF_GRAMMAR_TOKEN("{", '{')
DEF_GRAMMAR_TOKEN("}", '}')
DEF_GRAMMAR_TOKEN("(", '(')
DEF_GRAMMAR_TOKEN(")", ')')

DEF_GRAMMAR_TOKEN("The fallen leaves tell us a story",                 BEGINNING)
DEF_GRAMMAR_TOKEN("The Age of Fracture has come to the Lands Between", ENDING)

//== OPERATIONS ========================================================================//
DEF_OPERATION_TOKEN("And when the stars will fall along with",    IF_PREFIX)
DEF_OPERATION_TOKEN(", bless them with",                          IF_POSTFIX)
DEF_OPERATION_TOKEN("As I awaited",       WHILE_PREFIX)
DEF_OPERATION_TOKEN("return",             WHILE_POSTFIX)
DEF_OPERATION_TOKEN("is now",             ASSIGNMENT)
DEF_OPERATION_TOKEN("forged",             VAR_DEFINITION)

//== FUNCTIONS =========================================================================//
DEF_OPERATION_TOKEN("The grace of", FUNCTION_DEFINITION_PREFIX)
DEF_OPERATION_TOKEN("upon the",     FUNCTION_DEFINITION_POSTFIX)

DEF_OPERATION_TOKEN("Bless thy tarnished with", RETURN)

DEF_OPERATION_TOKEN("Let thy grace of", FUNCTION_CALL_PREFIX)
DEF_OPERATION_TOKEN("shine again upon", FUNCTION_CALL_POSTFIX)

//== MATH OPERATIONS ===================================================================//
DEF_OPERATION_TOKEN("smelted with",   ADD)
DEF_OPERATION_TOKEN("shadowed by",    SUB)
DEF_OPERATION_TOKEN("enchanted with", MUL)
DEF_OPERATION_TOKEN("fractured by",   DIV)
DEF_OPERATION_TOKEN("the ring of ",   VAR)

//== NUMBERS ===========================================================================//
DEF_NUM_TOKEN("rune of death",      0)
DEF_NUM_TOKEN("Starling amber",     1)
DEF_NUM_TOKEN("Lost ashes of war",  2)
DEF_NUM_TOKEN("Messmer's splinter", 3)
DEF_NUM_TOKEN("pure golden needle", 4)
DEF_NUM_TOKEN("Michella's needle",  5)
