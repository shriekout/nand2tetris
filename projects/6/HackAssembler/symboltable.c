#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symboltable.h"
#include "config.h"

static SymbolTable symbolTable[TABLE_MAX];
static int symbolCount = 0;


void initSymbolTable(void)
{
    symbolCount = 0;

    char buf[BUF_MAX];

    addSymbol("SP", 0);
    addSymbol("LCL", 1);
    addSymbol("ARG", 2);
    addSymbol("THIS", 3);
    addSymbol("THAT", 4);

    for (int i = 0; i < 16; i++) {
        snprintf(buf, BUF_MAX, "R%d", i);
        addSymbol(buf, i);
    }

    addSymbol("SCREEN", 16384);
    addSymbol("KBD", 24576);
}

void addSymbol(const char *symbol, int address)
{
    if (symbolCount >= TABLE_MAX) {
        fprintf(stderr, "Symbol table is full.\n");
        exit(1);
    }

    strncpy(symbolTable[symbolCount].symbol, symbol,SYMBOL_MAX);
    symbolTable[symbolCount].symbol[SYMBOL_MAX-1] = '\0';
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

int containsSymbol(const char *symbol)
{
    for (int i = 0; i < symbolCount; i++) {
        if (!strcmp(symbolTable[i].symbol, symbol))
            return 1;
    }

    return 0;
}

int getAddress(const char *symbol)
{
    for (int i = 0; i < symbolCount; i++) {
        if (!strcmp(symbolTable[i].symbol, symbol))
            return symbolTable[i].address;
    }

    fprintf(stderr, "Undefined symbol: %s\n", symbol);
    exit(1);
}
