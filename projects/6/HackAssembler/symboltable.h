#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "config.h"

typedef struct {
    char symbol[SYMBOL_MAX];
    int address;
} SymbolTable;

void initSymbolTable(void);
void addSymbol(const char*, int);
int containsSymbol(const char*);
int getAddress(const char*);

#endif