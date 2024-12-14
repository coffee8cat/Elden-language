#include "id_table.h"


identificator* prepare_ids_table()
{
    identificator* ids_table = (identificator*)calloc(max_num_of_ids, sizeof(identificator));
    if (!ids_table) { fprintf(stderr, "ERROR: Calloc for ids_table failed\n"); }
    return ids_table;
}

size_t add_id(identificator* ids_table, const char* name, size_t name_length)
{
    assert(ids_table);
    assert(name);

    printf("ids_table on %p\n", ids_table);
    size_t position = lookup(ids_table, name, name_length);
    printf("position = %d", position);
    printf("name: %p; starts with %c\n", name, *name);
    printf("name_length = %d\n", name_length);
    if (ids_table[position].name == NULL)
    {
        ids_table[position].name      = name;
        ids_table[position].name_len  = name_length;
    }
    dump_ids_table(ids_table);
    return position;
}

size_t lookup(identificator* ids_table, const char* name, size_t name_length)
{
    assert(ids_table);
    assert(name);

    size_t curr = 0;
    while (ids_table[curr].name && curr < max_num_of_ids)
    {
        if (strncmp(name, ids_table[curr].name, name_length) == 0)
            break;
        curr++;
    }
    return curr;
}

void dump_ids_table(identificator* ids_table)
{
    assert(ids_table);

    printf("\n---VARIABLES TABLE DUMP---\n");
    size_t i = 0;
    while (ids_table[i].name && i < max_num_of_ids)
    {
        printf("var in %p\n", &ids_table[i]);
        printf ("id%d: name: [%.*s] len = %d value: [%lg]\n",
                i, ids_table[i].name_len, ids_table[i].name, ids_table[i].name_len, ids_table[i].value);
        i++;
    }
    printf("\n--- DUMP END ---\n\n");
}
