
//== NUMBERS ===========================================================================//
DEF_TOKEN("rune of death",      LEX_NUM, ZERO)
DEF_TOKEN("Starling amber",     LEX_NUM, ONE)
DEF_TOKEN("Lost ashes of war",  LEX_NUM, TWO)
DEF_TOKEN("Messmer's splinter", LEX_NUM, THREE)
DEF_TOKEN("pure golden needle", LEX_NUM, FOUR)
DEF_TOKEN("Michella's needle",  LEX_NUM, FIVE)

//== OPERATIONS ========================================================================//
DEF_TOKEN("And when the stars will fall along with",    LEX_OP, IF_PREFIX)
DEF_TOKEN("bless them with",                            LEX_OP, IF_POSTFIX)
DEF_TOKEN("As I awaited",       LEX_OP, WHILE_PREFIX)
DEF_TOKEN("return",             LEX_OP, WHILE_POSTFIX)
DEF_TOKEN("is now",             LEX_OP, ASSIGNMENT_GRAMMAR)
DEF_TOKEN("forged",             LEX_OP, VAR_DEFINITION)
DEF_TOKEN("the ring of ",       LEX_OP, VAR_USAGE)

//== FUNCTIONS =========================================================================//
DEF_TOKEN("The grace of", LEX_OP, FUNCTION_DEFINITION_PREFIX)
DEF_TOKEN("upon",         LEX_OP, FUNCTION_DEFINITION_POSTFIX)

DEF_TOKEN("Bless thy tarnished with", LEX_OP, RETURN)

DEF_TOKEN("Let thy grace of", LEX_OP, FUNCTION_CALL_PREFIX)
DEF_TOKEN("shine again upon", LEX_OP, FUNCTION_CALL_POSTFIX)

//== MATH OPERATIONS ===================================================================//
DEF_TOKEN("smelted with",   LEX_OP, ADD)
DEF_TOKEN("shadowed by",    LEX_OP, SUB)
DEF_TOKEN("enchanted with", LEX_OP, MUL)
DEF_TOKEN("fractured by",   LEX_OP, DIV)

