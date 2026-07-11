#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <stdio.h>

void tokenize(FILE*, FILE*);

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