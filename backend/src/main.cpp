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

    FILE* saved_tree = fopen("..\\data\\sqsolver_tree.txt", "r");
    assert(saved_tree);

    const char* buff_for_read = readfile(saved_tree);
    fclose(saved_tree);
    check_signatures(&buff_for_read);

    identificator* ids_table_for_reading = prepare_ids_table();
    node_t* node_read = read_tree(ids_table_for_reading, &buff_for_read);

    change_on_defs(&node_read);
    dump_ids_table(ids_table_for_reading);
    tree_dump(node_read, ids_table_for_reading, html_stream, node_read);


    size_t global_vars_counter = 0;
    prepare_to_compile(node_read, ids_table_for_reading, &global_vars_counter);

    printf("number of global vars: %d\n", global_vars_counter);
    dump_ids_table(ids_table_for_reading);

    FILE* fact_fp = fopen("..\\data\\sqsolver.txt", "w");

    translate_OP(node_read, ids_table_for_reading, fact_fp, global_vars_counter);
    fprintf(fact_fp, "\n\nHLT\n");

    fclose(fact_fp);

    reverse_frontend(node_read, ids_table_for_reading);
    return 0;
}
