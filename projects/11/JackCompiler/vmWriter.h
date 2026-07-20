#ifndef WMWRITER_H
#define WMWRITER_H

#include <stdio.h>

#include "types.h"

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