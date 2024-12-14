#include "files_usage.h"

const char* readfile(FILE* input)
{
    assert(input);

    size_t file_size = get_file_size(input);
    char* buff = (char*)calloc(file_size, sizeof(char));
    if (buff == NULL) { fprintf(stderr, "ERROR: Calloc failed in %s:%d in %s\n", __FILE__, __LINE__, __func__); }

    fread(buff, sizeof(char), file_size, input);
    return buff;
}

size_t get_file_size(FILE* fp)
{
    assert(fp);

    struct stat st = {};
    fstat(fileno(fp), &st);
    return (size_t)st.st_size;
}
