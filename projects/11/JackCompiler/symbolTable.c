#include <string.h>

#include "symbolTable.h"

#define MAX_SYMBOLS 256

typedef struct {
    char name[MAX_BUF];
    char type[MAX_BUF];
    varKind kind;
    int index;
} Symbol;

typedef struct {
    Symbol table[MAX_SYMBOLS];
    int count;
} SymbolTable;

static struct VarCounts {
    int staticCount;
    int fieldCount;
    int argumentCount;
    int varCount;
} varCounts;

static SymbolTable classTable;
static SymbolTable subTable;

static Symbol *findSymbol(char *);

void define(char *name, char *type, varKind kind)
{
    SymbolTable *p;

    if (kind == k_static || kind == k_field)
        p = &classTable;
    else
        p = &subTable;
        
    strcpy(p->table[p->count].name, name);
    strcpy(p->table[p->count].type, type);
    p->table[p->count].kind = kind;

    switch (kind) {
        case k_none:
            break;

        case k_static:
            p->table[p->count].index = varCounts.staticCount++;
            break;

        case k_field:
            p->table[p->count].index = varCounts.fieldCount++;
            break;

        case k_argument:
            p->table[p->count].index = varCounts.argumentCount++;
            break;

        case k_var:
            p->table[p->count].index = varCounts.varCount++;
            break;
    }

    p->count++;
}

void initializeClassTable(void)
{
    classTable.count = 0;
    varCounts.staticCount = 0;
    varCounts.fieldCount = 0;
}

void initializeSubTable(void)
{
    subTable.count = 0;
    varCounts.argumentCount = 0;
    varCounts.varCount = 0;
}

int getVarCount(varKind kind)
{
    switch(kind) {
        case k_none:        return 0;

        case k_static:      return varCounts.staticCount;

        case k_field:       return varCounts.fieldCount;

        case k_argument:    return varCounts.argumentCount;

        case k_var:         return varCounts.varCount;
    }

    return 0;
}

varKind kindOf(char *name)
{
    Symbol *p = findSymbol(name);
    return p ? p->kind : k_none;
}

char *typeOf(char *name)
{
    Symbol *p = findSymbol(name);
    return p ? p->type : NULL;
}

int indexOf(char *name)
{
    Symbol *p = findSymbol(name);
    return p ? p->index : -1;
}

static Symbol *findSymbol(char *name)
{
        for (int i = 0; i < subTable.count; i++) {
        if (!strcmp(name, subTable.table[i].name)) {
            return &subTable.table[i];
        }
    }

    for (int i = 0; i < classTable.count; i++) {
        if (!strcmp(name, classTable.table[i].name)) {
            return &classTable.table[i];
        }
    }

    return NULL;
}