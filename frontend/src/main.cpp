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
    const char* buff = read_file_data();

    lexeme_t* cmds = string_to_lexemes(buff, ids_table);

    FILE* html_stream = prepare_to_dump();
    assert(html_stream);

    size_t curr = 0;
    node_t* root = get_General(cmds, ids_table, &curr, html_stream);
    tree_dump(root, ids_table, html_stream, root);

    save_tree_data(root, ids_table);

    fclose(html_stream);
    tree_dtor(root);
    ids_table_dtor(ids_table);
    return 0;
}
