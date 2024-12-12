#include "lexical_analysis.h"

int main()
{
    identificator* ids_table = prepare_ids_table();
    const char* buff = "Let thy grace of Michella shine again upon the ring of Godwin rune of death";
    for (size_t i = 0; i < 10; i++)
    {
        printf("%4d: %d\nvalue = %lg(%c)\n", i,
        cmds[i].type,
        cmds[i].value,
        (char)cmds[i].value);
    }
    dump_ids_tabel(ids_tabel);
    lexeme_t* cmds = string_to_lexemes(buff, ids_table);
    return 0;
}
