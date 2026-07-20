#include <stdio.h>

#include "vmWriter.h"
#include "types.h"

void writePush(FILE *fout, segment seg, int idx)
{
    char *s;

    switch (seg) {
        case CONSTANT:  s = "constant";    break;
        case ARGUMENT:  s = "argument";    break;
        case LOCAL:     s = "local";       break;
        case STATIC:    s = "static";      break;
        case THIS:      s = "this";        break;
        case THAT:      s = "that";        break;
        case POINTER:   s = "pointer";     break;
        case TEMP:      s = "temp";        break;
    }

    fprintf(fout, "push %s %d\n", s, idx);
}

void writePop(FILE *fout, segment seg, int idx)
{
    char *s;

    switch (seg) {
        case CONSTANT:  break;
        case ARGUMENT:  s = "argument";    break;
        case LOCAL:     s = "local";       break;
        case STATIC:    s = "static";      break;
        case THIS:      s = "this";        break;
        case THAT:      s = "that";        break;
        case POINTER:   s = "pointer";     break;
        case TEMP:      s = "temp";        break;
    }

    fprintf(fout, "pop %s %d\n", s, idx);
}

void writeArithmetic(FILE *fout, arithmetic cmd)
{
    char *s;

    switch (cmd) {
        case ADD:   s = "add";                  break;
        case SUB:   s = "sub";                  break;
        case MUL:   s = "call Math.multiply 2"; break;
        case DIV:   s = "call Math.divide 2";   break;
        case NEG:   s = "neg";                  break;
        case EQ:    s = "eq";                   break;
        case GT:    s = "gt";                   break;
        case LT:    s = "lt";                   break;
        case AND:   s = "and";                  break;
        case OR:    s = "or";                   break;
        case NOT:   s = "not";                  break;
    }

    fprintf(fout, "%s\n", s);
 
}

void writeLabel(FILE *fout, char *label)
{
    fprintf(fout, "label %s\n", label);
}

void writeGoto(FILE *fout, char *label)
{
    fprintf(fout, "goto %s\n", label);
}

void writeIf(FILE *fout, char *label)
{
    fprintf(fout, "if-goto %s\n", label);
}

void writeCall(FILE *fout, char *name, int nArgs)
{
    fprintf(fout, "call %s %d\n", name, nArgs);
}


void writeFunction(FILE *fout, char *name, int nVars)
{
    fprintf(fout, "function %s %d\n", name , nVars);
}

void writeReturn(FILE *fout)
{
    fprintf(fout, "return\n");
}