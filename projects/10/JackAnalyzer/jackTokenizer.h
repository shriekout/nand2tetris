#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <stdio.h>

#define BUF_MAX 256

typedef enum {
    TOKEN_EOF = -1,
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
} tokenType;

typedef enum {
    CLASS,
    METHOD,
    FUNCTION,
    CONSTRUCTOR,
    INT,
    BOOLEAN,
    CHAR,
    VOID,
    VAR,
    STATIC,
    FIELD,
    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    KW_NULL,
    THIS
} keywordType;

void tokenize(FILE*, FILE*);
// Read one token.
tokenType advance(FILE*, char*);
// Push back one token.
void pushBack(tokenType, const char *);

#endif