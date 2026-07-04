#ifndef CODEWRITE_H
#define CODEWRITE_H

#include <stdio.h>

#include "parser.h"

void writeBootstrap(FILE*);
void writeArithmetic(FILE *, const char*);
void writePush(FILE*, const char*, const int);
void writePop(FILE*, const char*, const int);
void writeLabel(FILE*, const char*);
void writeIF(FILE*, const char*);
void writeGoto(FILE*, const char*);

#endif