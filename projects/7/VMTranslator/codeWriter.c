#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "codeWriter.h"
#include "config.h"

// Initialize
// @256
// D=A
// @SP
// M=D
void Initialize(FILE *fout)
{
    char code[] = "@256\n"
                    "D=A\n"
                    "@SP\n"
                    "M=D\n";

    fprintf(fout, "%s", code);
}

void writeArithmetic(const char *cmd)
{
    printf("add\n");
}

// push constant 7
// @7
// D=A
// @SP
// A=M
// M=D
// @SP
// M=M+1
void codePush(FILE *fout, const char *seg, const int idx)
{
    char buf[BUF_MAX];

    if (!strcmp(seg, "constant")) {
        snprintf(buf, sizeof(buf),
                "@%d\n"
                "D=A\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n", 
                idx);
    }

    fprintf(fout, "%s", buf);
}

void writePop(const char *seg, const int idx)
{

}