#include <slug_parser.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void slug_parser_ast_alloc_stmt(Ast *ast, Stmt stmt)  {
    if (ast->size >= ast->cap) {
        ast->cap *= 2;
        ast->statements = realloc(ast->statements, ast->cap * sizeof(Stmt));
    }

    ast->statements[ast->size] = stmt;
    ast->size++;
}

static void slug_parser_advance(unsigned int* index) { (*index)++; }


static SlugToken slug_parser_peek(SlugToken* tokens, unsigned int* index) {
    SlugToken token = tokens[*index];
    slug_parser_advance(index);
    return token;
}

static void slug_parser_expected_symbol(SlugToken token, SlugTokenType expected_type)  {
    if (token.type != expected_type) {
        printf("Parser error: expected symbol type %s, got type %s\n", slug_type_to_string(expected_type), slug_type_to_string(token.type));
        exit(1);
    }
}

static SlugToken slug_parser_expected(SlugToken token, SlugTokenType expected_type) {
    if (token.type != expected_type) {
        printf("Parser error: expected %s, got %s\n",slug_type_to_string(expected_type), slug_type_to_string(token.type));
        exit(1);
    }
    return token;
}

static int slug_parser_expression_int(SlugToken *tokens, unsigned int *index) { 
    return slug_parser_peek(tokens, index).token_number;
}

Stmt slug_parser_stmt_declaration_variable(SlugToken *tokens, unsigned int *index) {
    Stmt stmt = {0};
    stmt.stmt_kind = StmtVariableDeclaration;

    slug_parser_advance(index);
    stmt.stmt_name = strdup(slug_parser_expected(slug_parser_peek(tokens, index), SlugTokenIdentifier).token_identifier);
    slug_parser_expected_symbol(slug_parser_peek(tokens, index), SlugTokenAssign);
    // TODO: handle types here 
    stmt.expr_kind.type = TypeValueInt;
    if (stmt.expr_kind.type == TypeValueInt) {
        stmt.expr_kind.expr_literal.literal_int32 = slug_parser_expression_int(tokens, index); 
    }
    slug_parser_expected_symbol(slug_parser_peek(tokens, index), SlugTokenSemicolon);
    return stmt;
}

Ast* slug_parser_parse(SlugToken *tokens, unsigned int *index) {
    Ast* ast = malloc(sizeof(Ast));
    ast->size = 0;
    ast->cap = 10;
    ast->statements = malloc(sizeof(Stmt*) * ast->cap);

    while (tokens[*index].type != SlugTokenEof) {

        printf("kind %s\n", slug_type_to_string(tokens[*index].type));
        if (tokens[*index].type == SlugTokenLet) {
            slug_parser_ast_alloc_stmt(ast, slug_parser_stmt_declaration_variable(tokens , index));
        } else {
            printf("Parser error: unknown token type %s\n", slug_type_to_string(slug_parser_peek(tokens, index).type));
            exit(1);
        }
    }
    return ast;
}