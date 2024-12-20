#include "middleend.h"
#include "backend.h"

#include "id_table.h"
#include "files_usage.h"
#include "tree_dump.h"
#include "reverse_frontend.h"
#include "exchange_tree.h"

int main()
{
    FILE* html_stream = prepare_to_dump();
    assert(html_stream);


    identificator* ids_table = prepare_ids_table();
    const char* buff_for_read = read_file_data();

    node_t* node_read = read_tree_data(ids_table, &buff_for_read, html_stream);

    size_t global_vars_counter = 0;
    prepare_to_compile(node_read, ids_table, &global_vars_counter);

    tree_to_asm(node_read, ids_table, global_vars_counter);

    reverse_frontend(node_read, ids_table);

    fclose(html_stream);
    tree_dtor(node_read);
    ids_table_dtor(ids_table);

    return 0;
}
