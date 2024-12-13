#include "lexical_analysis.h"
#include "id_table.h"

int main()
{
    identificator* ids_table = prepare_ids_table();
    const char* buff = "Let thy grace of Michella shine again upon the ring of Godwin rune of death";

    lexeme_t* cmds = string_to_lexemes(buff, ids_table);

    #define DEF_TOKEN(name, type, value) #value,

    const char* operations_list[] =
    {
        #include "DSL_elden.h"
    };

    #undef DEF_TOKEN

    for (size_t i = 0; i < 10; i++)
    {
        if (cmds[i].type == ID)
        {

            printf("%4d: %.*s(%d)\nposition = %d\n",
            i, ids_table[cmds[i].value].name_len, ids_table[cmds[i].value].name,
            cmds[i].type, cmds[i].value);
        }
        else
        {
            printf("%4d: %s(%d)\nvalue = %d(%c)\n",
            i, operations_list[cmds[i].value],
            cmds[i].type,
            cmds[i].value,
            (char)cmds[i].value);
        }
    }

    dump_ids_table(ids_table);
    return 0;
}
