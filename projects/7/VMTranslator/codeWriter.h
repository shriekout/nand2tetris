#ifndef CODEWRITE_H
#define CODEWRITE_H

#include <stdio.h>

#include "parser.h"

void Initialize(FILE*);
void writeArithmetic(const char*);
void codePush(FILE*, const char*, const int);
void writePop(const char*, const int);

#endif