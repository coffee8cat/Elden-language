#include "lexical_analysis.h"
#include "id_table.h"
#include "files_usage.h"
#include "tree_dump.h"
#include "syntax_analysis.h"
#include "middleend.h"
#include "backend.h"
#include "reverse_frontend.h"
#include "exchange_tree.h"

int main()
{
    identificator* ids_table = prepare_ids_table();

    FILE* input = fopen("data\\sqsolver.txt", "r");
    assert(input);
    const char* buff = readfile(input);
    fclose(input);
    printf("%s\n", buff);

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

    FILE* stream_for_save = fopen("data\\saved_tree.txt", "w");
    if (!stream_for_save) { fprintf(stderr, "ERROR: Could not open stream_for_save\n"); }

    fprintf(stream_for_save, "434-format v1.5\n");
    save_tree(root, ids_table, stream_for_save);
    reverse_frontend(root, ids_table);

    fclose(stream_for_save);

    FILE* saved_tree = fopen("data\\factorial_tree.txt", "r");
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

    FILE* fact_fp = fopen("data\\sqsolver_asm.txt", "w");

/*
    fprintf(fact_fp, "CALL EldenLord:\n");
    fprintf(fact_fp, "HLT\n\n");
*/
/*
    fprintf(fact_fp, "; Preparation\n");
    fprintf(fact_fp, "PUSH %d\n", global_vars_counter);
    fprintf(fact_fp, "POP BX\n");
*/
    translate_OP(node_read, ids_table_for_reading, fact_fp, global_vars_counter);

    fprintf(fact_fp, "\n\nHLT\n");

    fclose(fact_fp);


    return 0;
}
