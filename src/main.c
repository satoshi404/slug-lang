#include <slug_lexer.h>
#include <slug_io.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FileSource* source = read_file(argv[1]);
    
    SlugToken* tokens = slug_lexer_tokens(source->source, source->size);

    int i = 0;
    while (tokens[i].type != SlugTokenEof) {
        if (tokens[i].type == SlugTokenIdentifier) {
            printf("Identifier: %s\n", tokens[i].token_identifier);
        } else if (tokens[i].type == SlugTokenNumber) {
            printf("Number: %d\n", tokens[i].token_number);
        } else {
            printf("Token type: %s\n", slug_type_to_string(tokens[i].type));
        }
        i++;
    }

    // Liberar memória dos identificadores
    for (int j = 0; j < i; j++) {
        if (tokens[j].token_identifier != NULL) {
            free(tokens[j].token_identifier);
        }
    }
    free(tokens);

    free_file_source(source);
    return 0;
}
