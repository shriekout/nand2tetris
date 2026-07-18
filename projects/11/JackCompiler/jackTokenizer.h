#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <stdio.h>

#define MAX_BUF 256

typedef enum {
    TOKEN_EOF = -1,
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
} tokenType;

// Read one token.
tokenType advance(FILE*, char*);
// Push back one token.
void pushBack(tokenType, const char *);

#endif