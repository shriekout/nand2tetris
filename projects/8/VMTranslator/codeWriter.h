#ifndef CODEWRITE_H
#define CODEWRITE_H

#include <stdio.h>

#include "parser.h"

void Initialize(FILE*);
void writeArithmetic(FILE *, const char*);
void writePush(FILE*, const char*, const int);
void writePop(FILE*, const char*, const int);

#endif