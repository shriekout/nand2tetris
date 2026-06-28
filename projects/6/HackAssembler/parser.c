#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

void removeWhitespace(char *buf)
{
    char *src = buf;
    char *dst = buf;

    while (*src != '\0') {
        if (!isspace((unsigned char)*src))
            *dst++ = *src;

        src++;
    }

    *dst = '\0';
}

int isBlank(const char *buf)
{
    while (*buf != '\0') {
        if (!isspace((unsigned char)*buf))
            return 0;

        buf++;
    }

    return 1;
}

void removeComment(char *buf)
{
    char *p = strstr(buf, "//");

    if (p != NULL)
        *p = '\0';
}

void extractSymbol(const char *buf, char *tmp)
{
    int i = 1;

    if (buf[0] == '(') {
        while (buf[i] != ')' && buf[i] != '\0') {
            tmp[i-1] = buf[i];
            i++;
        }

        tmp[i-1] = '\0';
    } else if (buf[0] == '@') {
        strcpy(tmp, buf+1);
    }
}

InstructionType getInstructionType(const char *buf)
{
    if (buf[0] == '@')
        return A_INSTRUCTION;
    else if (buf[0] == '(')
        return L_INSTRUCTION;
    else
        return C_INSTRUCTION;
}

int isNumber(const char *s)
{
    while (*s) {
        if (!isdigit((unsigned char)*s))
            return 0;
        
        s++;
    }

    return 1;
}