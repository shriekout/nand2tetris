#include <stdio.h>

#include "vmWriter.h"

void writeFunction(FILE *fout, char *name, int nVars)
{
    fprintf(fout, "function %s %d\n", name , nVars);
}