#include <string.h>

#include "symbolTable.h"
#include "jackTokenizer.h"

typedef struct {
    char name[32];
    char type[32];
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