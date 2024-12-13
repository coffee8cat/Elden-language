
//== NUMBERS ===========================================================================//
DEF_TOKEN("rune of death",      NUM, ZERO)
DEF_TOKEN("Starling amber",     NUM, ONE)
DEF_TOKEN("Lost ashes of war",  NUM, TWO)
DEF_TOKEN("Messmer's splinter", NUM, THREE)
DEF_TOKEN("pure golden needle", NUM, FOUR)
DEF_TOKEN("Michella's needle",  NUM, FIVE)

//== OPERATIONS ========================================================================//
DEF_TOKEN("And when the stars will fall along with",    OP, IF_PREFIX)
DEF_TOKEN("bless them with",                            OP, IF_POSTFIX)
DEF_TOKEN("As I awaited",       OP,WHILE_PREFIX)
DEF_TOKEN("return",             OP, WHILE_POSTFIX)
DEF_TOKEN("is now",             OP, ASSIGNMENT)
DEF_TOKEN("forged",             OP, VAR_DEFINITION)
DEF_TOKEN("the ring of ",       OP, VAR_USAGE)

//== FUNCTIONS =========================================================================//
DEF_TOKEN("The grace of", OP, FUNCTION_DEFINITION_PREFIX)
DEF_TOKEN("upon",         OP, FUNCTION_DEFINITION_POSTFIX)

DEF_TOKEN("Bless thy tarnished with", OP, RETURN)

DEF_TOKEN("Let thy grace of", OP, FUNCTION_CALL_PREFIX)
DEF_TOKEN("shine again upon", OP, FUNCTION_CALL_POSTFIX)

//== MATH OPERATIONS ===================================================================//
DEF_TOKEN("smelted with",   OP, ADD)
DEF_TOKEN("shadowed by",    OP, SUB)
DEF_TOKEN("enchanted with", OP, MUL)
DEF_TOKEN("fractured by",   OP, DIV)

