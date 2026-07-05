#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "codeWriter.h"
#include "config.h"

static int labelCount = 0;
static char currentFunction[BUF_MAX];

void codeWrite(FILE *fout, char *buf)
{
    char *cmd, *arg1, *arg2;
    commandType type;
    
    splitCmd(buf, &cmd, &arg1, &arg2);
    
    type = getCommandType(cmd);

    switch (type) {
        case C_ARITHMETIC: {
            writeArithmetic(fout, cmd);
            break;
        }

        case C_PUSH: {
            int idx = atoi(arg2);
            writePush(fout, arg1, idx);
            break;
        }

        case C_POP: {
            int idx = atoi(arg2);
            writePop(fout, arg1, idx);
            break;
        }

        case C_LABEL: {
            writeLabel(fout, arg1);
            break;
        }

        case C_IF: {
            writeIF(fout, arg1);
            break;
        }

        case C_GOTO: {
            writeGoto(fout, arg1);
            break;
        }

        case C_FUNCTION: {
            int nVars = atoi(arg2);
            writeFunction(fout, arg1, nVars);
            break;
        }

        default:
            fprintf(stderr, "%s: Unknown Command: %s\n", g_filename, cmd);
            exit(1);
    }
}

void writeBootstrap(FILE *fout)
{
    fprintf(fout, 
            "@256\n"
            "D=A\n"
            "@SP\n"
            "M=D\n");
}

void writeArithmetic(FILE *fout, const char *cmd)
{
    const char *buf;

    if (!strcmp(cmd, "add") || !strcmp(cmd, "sub")
        || !strcmp(cmd, "and") || !strcmp(cmd, "or")) {
        if (!strcmp(cmd, "add"))
            buf = "M=M+D";
        else if (!strcmp(cmd, "sub"))
            buf = "M=M-D";
        else if (!strcmp(cmd, "and"))
            buf = "M=M&D";
        else if (!strcmp(cmd, "or"))
            buf = "M=M|D";
        fprintf(fout,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "%s\n",
                buf);
    } else if (!strcmp(cmd, "neg")) {
        fprintf(fout,
                "@SP\n"
                "A=M-1\n"
                "M=-M\n");
    } else if (!strcmp(cmd, "not")) {
        fprintf(fout,
                "@SP\n"
                "A=M-1\n"
                "M=!M\n");
    } else if (!strcmp(cmd, "eq") 
                || !strcmp(cmd, "gt") 
                || !strcmp(cmd, "lt")) {
        int id = labelCount++;

        if (!strcmp(cmd, "eq"))
            buf = "JEQ";
        else if (!strcmp(cmd, "gt"))
            buf = "JGT";
        else if (!strcmp(cmd, "lt"))
            buf = "JLT";

        fprintf(fout, 
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@TRUE%d\n"
                "D;%s\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "@END%d\n"
                "0;JMP\n"
                "(TRUE%d)\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "(END%d)\n",
                id, buf, id, id, id);
    } else {
        fprintf(stderr, "Unknown Arithmetic command: %s\n", cmd);
        exit(EXIT_FAILURE);
    }
}

void writePush(FILE *fout, const char *seg, const int idx)
{
    const char *buf;

    if (!strcmp(seg, "constant")) {
        fprintf(fout, 
                "@%d\n"
                "D=A\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", 
                idx);
    } else if (!strcmp(seg, "local") || !strcmp(seg, "argument")
                || !strcmp(seg, "this") || !strcmp(seg, "that")) { 
        if (!strcmp(seg, "local"))
            buf = "LCL";
        else if (!strcmp(seg, "argument"))
            buf = "ARG";
        else if (!strcmp(seg, "this"))
            buf = "THIS";
        else if (!strcmp(seg, "that"))
            buf = "THAT";

        fprintf(fout,
                "@%d\n"
                "D=A\n"
                "@%s\n"
                "A=M+D\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n",
                idx, buf);
    } else if (!strcmp(seg, "temp")) {
        fprintf(fout,
                "@5\n"
                "D=A\n"
                "@%d\n"
                "A=D+A\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n",
                idx);
    } else if (!strcmp(seg, "pointer")) {
        if (idx == 0)
            buf = "THIS";
        else if (idx == 1)
            buf = "THAT";
        else {
            fprintf(stderr, "Unknown pointer idx\n");
            exit(1);
        }
        
        fprintf(fout,
                "@%s\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n",
                buf);
    } else if (!strcmp(seg, "static")) {
        fprintf(fout,
                "@%s.%d\n"
                "D=M\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n",
                g_filename, idx);
    } else {
        fprintf(stderr, "Unknown push segment: %s\n", seg);
        exit(EXIT_FAILURE);
    }
}

void writePop(FILE *fout, const char *seg, const int idx)
{
    const char *buf;

    if (!strcmp(seg, "local") || !strcmp(seg, "argument")
        || !strcmp(seg, "this") || !strcmp(seg, "that")) {
        if (!strcmp(seg, "local"))
            buf = "LCL";
        else if (!strcmp(seg, "argument"))
            buf = "ARG";
        else if (!strcmp(seg, "this"))
            buf = "THIS";
        else if (!strcmp(seg, "that"))
            buf = "THAT";

        fprintf(fout, 
                "@%s\n"
                "D=M\n"
                "@%d\n"
                "D=D+A\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n",
                buf, idx);
    } else if (!strcmp(seg, "temp")) {
        fprintf(fout,
                "@5\n"
                "D=A\n"
                "@%d\n"
                "D=D+A\n"
                "@R13\n"
                "M=D\n"
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@R13\n"
                "A=M\n"
                "M=D\n",
                idx);
    } else if (!strcmp(seg, "pointer")) {
        if (idx == 0)
            buf = "THIS";
        else if (idx == 1)
            buf = "THAT";
        else {
            fprintf(stderr, "Unknown pointer idx\n");
            exit(1);
        }

        fprintf(fout,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@%s\n"
                "M=D\n",
                buf);
    } else if (!strcmp(seg, "static")) {
        fprintf(fout,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "@%s.%d\n"
                "M=D\n",
                g_filename, idx);
    } else {
        fprintf(stderr, "Unknown pop segment: %s\n", seg);
        exit(EXIT_FAILURE);
    }
}

void writeLabel(FILE *fout, const char *arg1)
{
    fprintf(fout, "(%s$%s)\n", currentFunction, arg1);
}

void writeIF(FILE *fout, const char *arg1)
{
    fprintf(fout, 
            "@SP\n"
            "AM=M-1\n"
            "D=M\n"
            "@%s$%s\n"
            "D;JNE\n",
            currentFunction, arg1);
}

void writeGoto(FILE *fout, const char *arg1)
{
    fprintf(fout,
            "@%s$%s\n"
            "0;JMP\n",
            currentFunction, arg1);
}

void writeFunction(FILE *fout, const char *funName, const int nVars)
{
    strcpy(currentFunction, funName);

    fprintf(fout, "(%s)\n", funName);

    for (int i = 0; i < nVars; i++) {
        char buf[] = "push constant 0";
        codeWrite(fout, buf);
    }
}