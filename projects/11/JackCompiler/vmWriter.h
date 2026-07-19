#ifndef WMWRITER_H
#define WMWRITER_H

#include <stdio.h>

#define MAX_BUF 256

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

void writePush(FILE*, segment, int);
void writePop(FILE*, segment, int);
void writeArithmetic(FILE*, arithmetic);
void writeLabel(FILE*, char*);
void writeGoto(FILE*, char*);
void writeIf(FILE*, char*);
void writeCall(FILE*, char*, int);
void writeFunction(FILE*, char*, int);
void writeReturn(FILE*);

#endif