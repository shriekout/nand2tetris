#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"

void getBinary(const int addr, char *tmp)
{
    for (int i = 15; i >= 0; i--) {
        *tmp++ = ((addr >> i) & 1) + '0';
    }

    *tmp = '\0';
}

const char *compBits(const char *buf)
{
    if (!strcmp(buf, "0"))          // a=0
        return "0101010";
    else if (!strcmp(buf, "1"))
        return "0111111";
    else if (!strcmp(buf, "-1"))
        return "0111010";
    else if (!strcmp(buf, "D"))
        return "0001100";
    else if (!strcmp(buf, "A"))
        return "0110000";
    else if (!strcmp(buf, "!D"))
        return "0001101";
    else if (!strcmp(buf, "!A"))
        return "0110001";
    else if (!strcmp(buf, "-D"))
        return "0001111";
    else if (!strcmp(buf, "-A"))
        return "0110011";
    else if (!strcmp(buf, "D+1"))
        return "0011111";
    else if (!strcmp(buf, "A+1"))
        return "0110111";
    else if (!strcmp(buf, "D-1"))
        return "0001110";
    else if (!strcmp(buf, "A-1"))
        return "0110010";
    else if (!strcmp(buf, "D+A"))
        return "0000010";
    else if (!strcmp(buf, "D-A"))
        return "0010011";
    else if (!strcmp(buf, "A-D"))
        return "0000111";
    else if (!strcmp(buf, "D&A"))
        return "0000000";
    else if (!strcmp(buf, "D|A"))
        return "0010101";
    else if (!strcmp(buf, "M"))         // a=1
        return "1110000";
    else if (!strcmp(buf, "!M"))
        return "1110001";
    else if (!strcmp(buf, "-M"))
        return "1110011";
    else if (!strcmp(buf, "M+1"))
        return "1110111";
    else if (!strcmp(buf, "M-1"))
        return "1110010";
    else if (!strcmp(buf, "D+M"))
        return "1000010";
    else if (!strcmp(buf, "D-M"))
        return "1010011";
    else if (!strcmp(buf, "M-D"))
        return "1000111";
    else if (!strcmp(buf, "D&M"))
        return "1000000";
    else if (!strcmp(buf, "D|M"))
        return "1010101";

    fprintf(stderr, "Unknown Comp!!!\n");

    exit(EXIT_FAILURE);
}

const char *destBits(const char *buf)
{
    if (buf[0] == '\0')
        return "000";
    else if (!strcmp(buf, "M"))
        return "001";
    else if (!strcmp(buf, "D"))
        return "010";
    else if (!strcmp(buf, "DM"))
        return "011";
    else if (!strcmp(buf, "MD"))    // same upper
        return "011";
    else if (!strcmp(buf, "A"))
        return "100";
    else if (!strcmp(buf, "AM"))
        return "101";
    else if (!strcmp(buf, "AD"))
        return "110";
    else if (!strcmp(buf, "ADM"))
        return "111";
    
    fprintf(stderr, "Unknown Dest!!!\n");

    exit(EXIT_FAILURE);
}

const char *jumpBits(const char *buf)
{
    if (buf[0] == '\0')
        return "000";
    else if (!strcmp(buf, "JGT"))
        return "001";
    else if (!strcmp(buf, "JEQ"))
        return "010";
    else if (!strcmp(buf, "JGE"))
        return "011";
    else if (!strcmp(buf, "JLT"))
        return "100";
    else if (!strcmp(buf, "JNE"))
        return "101";
    else if (!strcmp(buf, "JLE"))
        return "110";
    else if (!strcmp(buf, "JMP"))
        return "111";

    fprintf(stderr, "Unknown JUMP!!!\n");

    exit(EXIT_FAILURE);
}
