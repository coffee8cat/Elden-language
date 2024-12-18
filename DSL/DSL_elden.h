
//== NUMBERS ===========================================================================//
DEF_TOKEN("rune of Death",      NUM, ZERO)
DEF_TOKEN("Starling amber",     NUM, ONE)
DEF_TOKEN("Lost ashes of war",  NUM, TWO)
DEF_TOKEN("Messmer's splinter", NUM, THREE)
DEF_TOKEN("pure golden needle", NUM, FOUR)
DEF_TOKEN("Michella's needle",  NUM, FIVE)

//== OPERATIONS ========================================================================//
DEF_TOKEN("And when the stars will fall along with",    OP, IF_PREFIX)
DEF_TOKEN("bless them with",                            OP, IF_POSTFIX)
DEF_TOKEN("As I awaited",       OP, WHILE_PREFIX)
DEF_TOKEN("return",             OP, WHILE_POSTFIX)

DEF_TOKEN("And now",            OP, ASSIGNMENT_PREFIX)
DEF_TOKEN("is",                 OP, ASSIGNMENT_INFIX)

DEF_TOKEN("forged",             OP, VAR_DEFINITION_POSTFIX)
DEF_TOKEN("the ring of",        OP, VAR_USAGE_PREFIX)

DEF_TOKEN("In the Lands Between was found",      OP, SCAN_PREFIX)
DEF_TOKEN("In the age of Duskborn",             OP, PRINT_PREFIX)
DEF_TOKEN("will thunder in the darkest night",  OP, PRINT_POSTFIX)

//== FUNCTIONS =========================================================================//
DEF_TOKEN("The fallen leaves tell us a story of the",   OP, FUNCTION_DEFINITION_PREFIX)
DEF_TOKEN("Let thy grace of",                           OP, FUNCTION_CALL_PREFIX)
DEF_TOKEN("shine upon",                                 OP, SPECIFICATION_INFIX)

DEF_TOKEN("Bless thy tarnished with",                   OP, RETURN_PREFIX)

//== MATH OPERATIONS ===================================================================//
DEF_TOKEN("smelted with",                   OP, ADD_LEX)
DEF_TOKEN("shadowed by",                    OP, SUB_LEX)
DEF_TOKEN("enchanted with",                 OP, MUL_LEX)
DEF_TOKEN("fractured by",                   OP, DIV_LEX)
DEF_TOKEN("arised with rune arc made from", OP, POW_LEX)

