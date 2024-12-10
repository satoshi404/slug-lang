#include <slug_lexer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define BUFFER_SIZE_IDENT 255

char* slug_type_to_string(SlugTokenType type) {
    switch (type) {
        case SlugTokenLet: return "let";
        case SlugTokenNumber: return "number";
        case SlugTokenPlus: return "+";
        case SlugTokenMinus: return "-";
        case SlugTokenMultiply: return "*";
        case SlugTokenDivide: return "/";
        case SlugTokenAssign: return "=";
        case SlugTokenOpenParen: return "(";
        case SlugTokenCloseParen: return ")";
        case SlugTokenOpenBrace: return "{";
        case SlugTokenCloseBrace: return "}";
        case SlugTokenComma: return ",";
        case SlugTokenReturn: return "return";
        case SlugTokenColon: return ":";
        case SlugTokenSemicolon: return ";";
        case SlugTokenIdentifier: return "identifier";
        case SlugTokenFunction: return "function";
        case SlugTokenEof: return "eof";
    }
}

static void reallocator_tokens(SlugToken** tokens, SlugToken token, size_t* size, size_t* capacity) {
    if (*size >= *capacity) {
        *capacity *= 2;
        *tokens = realloc(*tokens, *capacity * sizeof(SlugToken));
    }

    (*tokens)[*size] = token;
    (*size)++;
}

static bool slug_lexer_is_whitespace(char** file_content, unsigned int line, unsigned int* column) {
    char symbol = file_content[line][*column];
    switch (symbol) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            (*column)++;
            return true;
    }
    return false;
}

static SlugTokenType slug_lexer_get_symbol(char** file_content, unsigned int line, unsigned int* column) {
    char symbol = file_content[line][*column];
    switch (symbol) {
        case '+': (*column)++;  return SlugTokenPlus;
        case '-': (*column)++;  return SlugTokenMinus;
        case '*': (*column)++;  return SlugTokenMultiply;
        case '/': (*column)++;  return SlugTokenDivide;
        case '=': (*column)++;  return SlugTokenAssign;
        case ',': (*column)++;  return SlugTokenComma;
        case ':': (*column)++;  return SlugTokenColon;
        case '(': (*column)++;  return SlugTokenOpenParen;
        case ')': (*column)++;  return SlugTokenCloseParen;
        case '{': (*column)++;  return SlugTokenOpenBrace;
        case '}': (*column)++;  return SlugTokenCloseBrace;
        case ';': (*column)++;  return SlugTokenSemicolon;
        default: return SlugTokenUnknown;
    }
}

static SlugTokenType slug_lexer_get_keywords(char** file_content, unsigned int line, unsigned int* column) {
    if (strncmp(&file_content[line][*column], "let", 3) == 0) {
        (*column) += 3;
        return SlugTokenLet;
    } else if (strncmp(&file_content[line][*column], "return", 6) == 0) {
        (*column) += 6;
        return SlugTokenReturn;
    } else if (strncmp(&file_content[line][*column], "function", 8) == 0) {
        (*column) += 8;
        return SlugTokenFunction;
    } else if (strncmp(&file_content[line][*column], "//", 2) == 0) {
        (*column) += 2;
        return SlugTokenComment;
    } else if (isalpha(file_content[line][*column]) || file_content[line][*column] == '-') { 
        return SlugTokenIdentifier;
    }
    return SlugTokenUnknown;
}

static SlugTokenType slug_lexer_get_number(char** file_content, unsigned int line, unsigned int* column) {
    if (isdigit(file_content[line][*column])) {
        return SlugTokenNumber;
    }
    return SlugTokenUnknown;
}

static char* slug_lexer_identifier(char** file_content, unsigned int line, unsigned int* column) {
    char* identifier = malloc(sizeof(char) * BUFFER_SIZE_IDENT);
    int i = 0;
    while (isalnum(file_content[line][*column]) || (file_content[line][*column] == '_' && file_content[line][*column] != '\0')) {
        identifier[i] = file_content[line][*column];
        i++;
        (*column)++;
    }
    identifier[i] = '\0';
    return identifier;
}

static int slug_lexer_integer(char** file_content, unsigned int line, unsigned int* column) {
    int number = 0;
    while (isdigit(file_content[line][*column])) {
        number = number * 10 + (file_content[line][*column] - '0');
        (*column)++;
    }
    return number;
}

static void slug_lexer_comment(char** file_content, unsigned int line, unsigned int* column) {
    while (file_content[line][*column] != '\0' && file_content[line][*column] != '\n') {
        (*column)++;
    }
}

static SlugToken* slug_lexer_tokenize(char** file_content, size_t size)  {
    size_t capacity_tokens = 100;
    size_t size_tokens = 0;
    SlugToken* tokens = malloc(capacity_tokens * sizeof(SlugToken));
    unsigned int line = 0, column = 0;

    while (line < size) {
            while (file_content[line][column] != '\0') {
                SlugTokenType type = SlugTokenUnknown;

                if (slug_lexer_is_whitespace(file_content, line, &column)) { continue; }

                if ((type = slug_lexer_get_keywords(file_content, line, &column)) != SlugTokenUnknown ||
                    (type = slug_lexer_get_symbol(file_content, line, &column)) != SlugTokenUnknown ||
                    (type = slug_lexer_get_number(file_content, line, &column)) != SlugTokenUnknown) {
                    
                    if (type == SlugTokenIdentifier) {
                        reallocator_tokens(&tokens, (SlugToken){
                            .type = type,
                            .token_number = 0,
                            .token_identifier = slug_lexer_identifier(file_content, line, &column)},
                            &size_tokens,
                            &capacity_tokens);   
                        continue;
                    } else if (type == SlugTokenNumber) {
                        reallocator_tokens(&tokens, (SlugToken){
                            .type = type,
                            .token_number = slug_lexer_integer(file_content, line, &column),
                            .token_identifier = NULL},
                            &size_tokens,
                            &capacity_tokens);
                        continue;
                    } else if (type == SlugTokenComment) {
                        slug_lexer_comment(file_content, line, &column);
                        continue;
                    } else {
                        reallocator_tokens(&tokens, (SlugToken){
                            .type = type,
                            .token_number = 0,
                            .token_identifier = NULL},
                            &size_tokens,
                            &capacity_tokens); 
                    }
                    
                }
                
                if (type == SlugTokenUnknown) {
                     reallocator_tokens(&tokens, (SlugToken) {
                        .type = type,
                        .token_number = 0,
                        .token_identifier = NULL},
                        &size_tokens,
                        &capacity_tokens);
                }
                column++;
            }
            column = 0;
            line++;
    }
      
    reallocator_tokens(&tokens, (SlugToken){
        .type = SlugTokenEof,
        .token_number = 0,
        .token_identifier = NULL}, 
        &size_tokens, 
        &capacity_tokens);
    
    return tokens;
}

SlugToken* slug_lexer_tokens(char** file_content, size_t size) {
    return slug_lexer_tokenize(file_content, size);
}
