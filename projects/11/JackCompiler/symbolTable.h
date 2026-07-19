#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define MAX_SYMBOLS 256

typedef enum {
    k_static,
    k_field,
    k_argument,
    k_var,
} varKind;

void define(char*, char*, varKind);
void initializeClassTable(void);
void initializeSubTable(void);
int getVarCount(varKind);

#endif