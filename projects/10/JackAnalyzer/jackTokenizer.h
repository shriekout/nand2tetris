#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <stdio.h>

#define BUF_MAX 256

void tokenize(FILE*, FILE*);
int advance(FILE*, char*);

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

#endif