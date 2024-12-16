#ifndef _IDENTIFICATOR_TABLE_H__
#define _IDENTIFICATOR_TABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

enum scope_type {GLOBAL, LOCAL};

//== IDS TABLE ========================================================================//
struct identificator
{
    // general
    const char* name;
    size_t      name_len;

    // for variables
    size_t      address;
    scope_type  scope;

    // for functions
    size_t      num_of_params;
    size_t      BX_shift;
    bool        is_defined;
};

const size_t max_num_of_ids = 128;

identificator* prepare_ids_table();
size_t add_id(identificator* ids_table, const char* name, size_t name_length);
//void ask_variables_values(identificator* ids_table);
size_t lookup(identificator* ids_table, const char* name, size_t name_length);
void dump_ids_table(identificator* ids_table);

#endif
