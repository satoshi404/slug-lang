#include <slug_parser.h>
#include <slug_io.h>
#include <stdio.h>
#include <stdlib.h>

static void print_ast(Ast* ast) {
    for (int i = 0; i < ast->size; i++) {
        if (ast->statements[i].stmt_kind == StmtVariableDeclaration) {
            printf("Variable declaration: %s\n", ast->statements[i].stmt_name);
            printf("Type: %s\n", slug_type_to_string(ast->statements[i].expr_kind.type));
            printf("Value: %d\n", ast->statements[i].expr_kind.expr_literal.literal_int32);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FileSource* source = read_file(argv[1]);
    SlugToken* tokens = slug_lexer_tokens(source->source, source->size);

    unsigned int index = 0;
    Ast* ast = slug_parser_parse(tokens, &index);
    printf("Parsed AST:\n");
    print_ast(ast);

  
    // Liberar memória dos identificadores
    for (int j = 0; j < index; j++) {
        if (tokens[j].token_identifier != NULL) {
            free(tokens[j].token_identifier);
        }
    }
    free(tokens);

    free_file_source(source);
    return 0;
}
