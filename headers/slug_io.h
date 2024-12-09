#ifndef SLUG_IO_H
#define SLUG_IO_H

#include <stdio.h>

typedef struct FileSource FileSource;

struct FileSource {
    char** source;
    size_t size;
    size_t capacity;
};

FileSource* read_file(const char* path);
void free_file_source(FileSource* source);

#endif // SLUG_IO_H