#include "lexical_analysis.h"
#include "syntax_analysis.h"

#include "id_table.h"
#include "files_usage.h"
#include "tree_dump.h"
#include "reverse_frontend.h"
#include "exchange_tree.h"

int main()
{
    identificator* ids_table = prepare_ids_table();

    FILE* input = fopen("..\\data\\sqsolver_elden.txt", "r");
    assert(input);

    const char* buff = readfile(input);
    fclose(input);
    printf("%s\n\n", buff);

    printf("\nStaring translating to lexemes\n\n");
    lexeme_t* cmds = string_to_lexemes(buff, ids_table);

    dump_lexemes_array(cmds, ids_table);
    dump_ids_table(ids_table);

    size_t curr = 0;
    FILE* html_stream = prepare_to_dump();
    assert(html_stream);

    node_t* root = get_General(cmds, ids_table, &curr, html_stream);

    //написать функцию распечатки синтаксического дерева
    tree_dump(root, ids_table, html_stream, root);

    FILE* stream_for_save = fopen("..\\data\\sqsolver_tree.txt", "w");
    if (!stream_for_save) { fprintf(stderr, "ERROR: Could not open stream_for_save\n"); }

    fprintf(stream_for_save, "434-format v1.5\n");

    save_tree(root, ids_table, stream_for_save);

    fclose(stream_for_save);

    return 0;
}
