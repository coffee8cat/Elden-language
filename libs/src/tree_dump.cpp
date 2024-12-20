#include "tree_dump.h"


FILE* prepare_to_dump()
{
    //system("make clean_dump\n");
    FILE* fp = fopen("list_dump.html", "w");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open html file fot dump\n");
        return NULL;
    }
    fprintf(fp, "<pre>\n");

    return fp;
}

int tree_dump(node_t* root, identificator* ids_table, FILE* html_stream, node_t* curr)
{
    static size_t dump_counter = 1;
    char dot_file_name[BUFSIZ] = "";
    char png_file_name[BUFSIZ] = "";
    sprintf(dot_file_name, "dump\\dump%d.dot", dump_counter);
    sprintf(png_file_name, "dump\\dump%d.png", dump_counter);

    FILE* fp = fopen(dot_file_name, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: Unable to open file for dump\n");
        return -1;
    }

    make_dot_file(root, ids_table, fp, curr);
    fclose(fp);

    char command[BUFSIZ] = "";
    sprintf(command, "dot %s -Tpng -o %s\n", dot_file_name, png_file_name);
    system(command);

    dump_to_html(root, png_file_name, html_stream, curr);

    dump_counter++;
    return 0;
}

int dump_to_html(node_t* root, char* png_file_name, FILE* html_stream, node_t* curr)
{
    assert(root);
    assert(png_file_name);

    fprintf(html_stream, "MEMORY PRINT\n");
    printf("\ncurr = %p\n", curr);

    fprintf(html_stream, "\n<img src=%s>\n", png_file_name);

    return 0;
}

int make_dot_file(node_t* root, identificator* ids_table, FILE* fp, node_t* curr)
{
    assert(fp);
    assert(root);

    fprintf(fp, "digraph\n{\n"
                "    rankdir=TP\n\n"
                "    ");

    write_node_info(root, ids_table, fp, curr);

    fprintf(fp, "}");

    printf("dot file completed\n");
    return 0;
}

#define DEF_OPERATION(enum_name, dump_name) dump_name,

const char* dump_operations_list[] =
{
    #include "DSL_elden_operations.h"
};
#undef DEF_OPERATION

int write_node_info(node_t* node, identificator* ids_table, FILE* fp, node_t* curr)
{
    assert(fp);

    if (!node) { return 1; }

    const char* color = NULL;
    char value[16] = {};
    char type[5]   = {};

    if (node -> type == OP)
    {
        color = "#F2900A";
        if (node -> value.op < END) { strcpy(value, dump_operations_list[node -> value.op]); }
        else { sprintf(value, "%c", node -> value.op); }
        strcpy(type, "OP");
    }
    if (node -> type == ID)
    {
        color = "#1BECC5";
        strncpy(value, ids_table[node -> value.id].name, ids_table[node -> value.id].name_len);
        strcpy(type, "ID");
    }
    if (node -> type == NUM)
    {
        color = "#0AF212";
        sprintf(value, "%lf", node -> value.num);
        strcpy(type, "NUM");
    }

    if (node == curr)       { color = "#BF62FC"; }

    fprintf(fp, "    node%p[shape=record,style=\"rounded,filled\",fillcolor=\"%s\",label=\"{ ptr: %p | type: %s | value: %s| { left: %p | right: %p }}\"];\n",
            node, color, node, type, value, node -> left, node -> right);

    if (node -> left)
    {
        fprintf(fp, "    node%p -> node%p[color=\"#0855F0\"]\n", node, node -> left);
        write_node_info(node -> left, ids_table, fp, curr);
    }
    if (node -> right)
    {
        fprintf(fp, "    node%p -> node%p[color=\"#0855F0\"]\n", node, node -> right);
        write_node_info(node -> right, ids_table, fp, curr);
    }

    return 0;
}
