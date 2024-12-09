#ifndef SLUG_LEXER_H
#define SLUG_LEXER_H
#include <stddef.h>
typedef struct SlugToken SlugToken;

typedef enum {
    SlugTokenLet,
    SlugTokenNumber,
    SlugTokenPlus,
    SlugTokenMinus,
    SlugTokenMultiply,
    SlugTokenDivide,
    SlugTokenAssign,
    SlugTokenOpenParen,
    SlugTokenCloseParen,
    SlugTokenOpenBrace,
    SlugTokenCloseBrace,
    SlugTokenComma,
    SlugTokenReturn,
    SlugTokenColon,
    SlugTokenSemicolon,
    SlugTokenIdentifier,
    SlugTokenFunction,
    SlugTokenUnknown,
    SlugTokenEof
} SlugTokenType;

struct SlugToken {
    char*     token_identifier;
    int       token_number;
    SlugTokenType     type;
};

SlugToken* slug_lexer_tokens(char** file_content, size_t size);

char* slug_type_to_string(SlugTokenType type);


#endif // SLUG_LEXER_H