#ifndef TYPES_H
#define TYPES_H

#define MAX_BUF 256

typedef enum {
    TOKEN_EOF = -1,
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST
} tokenType;


typedef enum {
    k_none = -1,
    k_static,
    k_field,
    k_argument,
    k_var,
} varKind;

typedef enum {
    CONSTANT,
    ARGUMENT,
    LOCAL,
    STATIC,
    THIS,
    THAT,
    POINTER,
    TEMP,
} segment;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    NEG,
    EQ,
    GT,
    LT,
    AND,
    OR,
    NOT,
} arithmetic;

#endif