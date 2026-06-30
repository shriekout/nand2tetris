#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "codeWriter.h"
#include "config.h"

static int labelCount = 0;

// Refer to the sand file.
void Initialize(FILE *fout)
{
    fprintf(fout, 
            "@256\n"
            "D=A\n"
            "@SP\n"
            "M=D\n");
}

// Refer to the sand file.
void writeArithmetic(FILE *fout, const char *cmd)
{
    const char *jump;

    if (!strcmp(cmd, "add")) {
        fprintf(fout,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "M=M+D\n");
    } else if (!strcmp(cmd, "sub")) {
        fprintf(fout,
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "M=M-D\n");

    } else if (!strcmp(cmd, "neg")) {
        fprintf(fout,
                "@SP\n"
                "A=M-1\n"
                "M=-M\n");
    } else if (!strcmp(cmd, "eq") 
                || !strcmp(cmd, "gt") 
                || !strcmp(cmd, "lt")) {
        int id = labelCount++;

        if (!strcmp(cmd, "eq"))
            jump = "JEQ";
        else if (!strcmp(cmd, "gt"))
            jump = "JGT";
        else if (!strcmp(cmd, "lt"))
            jump = "JLT";

        fprintf(fout, 
                "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "@TRUE%d\n"
                "D;%s\n"
                "@SP\n"
                "A=M\n"
                "M=0\n"
                "@END%d\n"
                "0;JMP\n"
                "(TRUE%d)\n"
                "@SP\n"
                "A=M\n"
                "M=-1\n"
                "(END%d)\n"
                "@SP\n"
                "M=M+1\n",
                id, jump, id, id, id);
    }
}

// Refer to the sand file.
void codePush(FILE *fout, const char *seg, const int idx)
{
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
    }
}

// Refer to the sand file.
void writePop(const char *seg, const int idx)
{

}