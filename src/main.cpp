#include "lexical_analysis.h"

int main()
{
    identificator* ids_table = prepare_ids_table();
    const char* buff = "";

    lexeme_t* cmds = string_to_lexemes(buff, ids_table);
    return 0;
}
