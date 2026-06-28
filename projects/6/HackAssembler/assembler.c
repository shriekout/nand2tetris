#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "assembler.h"
#include "parser.h"
#include "symboltable.h"
#include "code.h"

// removeComment -> isBlank -> removeWhitespace -> extractLabel
void firstPass(FILE *fin)
{
    char buf[BUF_MAX], tmp[BUF_MAX];
    int romAddr = 0;

    while(fgets(buf, BUF_MAX, fin) != NULL) {
        removeComment(buf);

        if (isBlank(buf))
            continue;

        removeWhitespace(buf);

        if (getInstructionType(buf) == L_INSTRUCTION) {
            extractSymbol(buf, tmp);
            addSymbol(tmp, romAddr);
        } else {
            romAddr++;
        }
    }
}

// removeComment -> isBlank -> removeWhitespace
void secondPass(FILE *fin, FILE *fout)
{
    char buf[BUF_MAX], tmp[BUF_MAX], tmp2[BUF_MAX];
    int addr = 16;
    int number;

    while(fgets(buf, BUF_MAX, fin) != NULL) {
        removeComment(buf);
        
        if (isBlank(buf))
            continue;

        removeWhitespace(buf);
        
        switch (getInstructionType(buf)) {
            case A_INSTRUCTION:
                extractSymbol(buf, tmp);

                if (containsSymbol(tmp)) {
                    getBinary(getAddress(tmp), tmp2);
                } else {
                    if (isNumber(tmp)) {
                        number = atoi(tmp);
                        getBinary(number, tmp2);
                    } else {
                        addSymbol(tmp, addr);
                        getBinary(addr, tmp2);
                        addr++;
                    }
                }

                break;
            case C_INSTRUCTION:
                strcpy(tmp2, "testC");
                break;
            case L_INSTRUCTION:
                strcpy(tmp2, "testL");
                break;
            default:
                break;
        }

        printf("%s\t%s\n", buf, tmp2);
    }
}