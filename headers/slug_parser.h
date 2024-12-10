#ifndef SLUG_PARSER_H
#define SLUG_PARSER_H

#include <slug_lexer.h>

typedef enum {
    StmtVariableDeclaration,
} KindStmt;

typedef enum {
    TypeValueInt,
} TypeExpr;

typedef struct {
    TypeExpr type;
    union slug_parser {
        int literal_int32;
    } expr_literal;
} Expr;

typedef struct {
    KindStmt stmt_kind;
    Expr expr_kind;
    char* stmt_name;
} Stmt;


typedef struct { 
    size_t size;
    size_t cap;  
    Stmt* statements;
} Ast;

Ast* slug_parser_parse(SlugToken *token, unsigned int *index);

#endif // SLUG_PARSER_H