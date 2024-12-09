#include <slug_io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE_LINE 1024

static void alloc_data(FileSource* source, char* buffer) {
    if (source->size >= source->capacity) {
        source->capacity *= 2;
        source->source = realloc(source->source, source->capacity * sizeof(char*));
    }
    source->source[source->size] = strdup(buffer);
    source->size++;
}

FileSource* read_file(const char* path) {
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        printf("Error opening file: %s\n", path);
        exit(1);
    }

    FileSource* source = (FileSource*) malloc(sizeof(FileSource));
    source->size = 0;
    source->capacity = 10; 
    source->source = (char**) malloc(sizeof(char*));

    char buffer[BUFFER_SIZE_LINE];

    while (fgets(buffer, sizeof(buffer), file)) {
        alloc_data(source, buffer);
    } 

    fclose(file);
    return source;
}

void free_file_source(FileSource* source) {
    for (size_t i = 0; i < source->size; i++) {
        free(source->source[i]);
    }
    free(source->source);
    free(source);
}
