#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "types.h"

void define(char*, char*, varKind);
void initializeClassTable(void);
void initializeSubTable(void);
int getVarCount(varKind);
varKind kindOf(char*);
char *typeOf(char*);
int indexOf(char*);

#endif