#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <stdio.h>

void parser(FILE*, FILE*);

typedef enum {
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
} keyword;

#endif