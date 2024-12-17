#include "lexical_analysis.h"
#include "id_table.h"
#include "files_usage.h"
#include "tree_dump.h"
#include "syntax_analysis.h"
#include "middleend.h"
#include "backend.h"

int main()
{
    identificator* ids_table = prepare_ids_table();

    FILE* input = fopen("data\\factorial.txt", "r");
    assert(input);
    const char* buff = readfile(input);

    lexeme_t* cmds = string_to_lexemes(buff, ids_table);

    dump_lexemes_array(cmds, ids_table);
    dump_ids_table(ids_table);

    size_t curr = 0;
    FILE* html_stream = prepare_to_dump();
    assert(html_stream);

    node_t* root = get_General(cmds, ids_table, &curr, html_stream);

    //написать функцию распечатки синтаксического дерева
    tree_dump(root, ids_table, html_stream, root);

    size_t global_vars_counter = 0;
    prepare_to_compile(root, ids_table, &global_vars_counter);

    printf("number of global vars: %d\n", global_vars_counter);
    dump_ids_table(ids_table);

    FILE* fact_fp = fopen("data\\fact.txt", "w");


    fprintf(fact_fp, "CALL EldenLord:\n");
    fprintf(fact_fp, "HLT\n\n");
    fprintf(fact_fp, "; Preparation\n");
    fprintf(fact_fp, "PUSH %d\n", global_vars_counter);
    fprintf(fact_fp, "POP BX\n");
    translate_OP(root, ids_table, fact_fp);

    return 0;
}
