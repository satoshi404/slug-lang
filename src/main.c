#include <slug_lexer.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char* file_content[] = {
        "let x = 5;"
    };
    size_t size = sizeof(file_content) / sizeof(file_content[0]);
    SlugToken* tokens = slug_lexer_tokens(file_content, size);

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
    return 0;
}
