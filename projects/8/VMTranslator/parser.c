#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "config.h"
#include "parser.h"
#include "codeWriter.h"

// removeComment -> isBlank
void parser(FILE *fin, FILE *fout)
{
    char buf[BUF_MAX];

    while (fgets(buf, BUF_MAX, fin) != NULL) {
        removeComment(buf);

        if (isBlank(buf))
            continue;

        codeWrite(fout, buf);
    }
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

void ltrim(char *buf)
{
    char *p = buf;

    while (isspace((unsigned char)*p))
        p++;

    if (p != buf)
        memmove(buf, p, strlen(p)+1);
}

void rtrim(char *buf)
{
    int len = strlen(buf);

    while (len > 0 && isspace((unsigned char)buf[len-1]))
        len--;

    buf[len] = '\0';
}

void trim(char *buf)
{
    ltrim(buf);
    rtrim(buf);
}

commandType getCommandType(const char *cmd)
{
    const char *arith[] = {
        "add", "sub", "neg", "eq", "gt", "lt", "and", "or", "not"
    };
    int n;

    n = sizeof(arith) / sizeof(arith[0]);

    for (int i = 0; i < n; i++) {
        if (!strcmp(cmd, arith[i]))
            return C_ARITHMETIC;
    }

    if (!strcmp(cmd, "push"))       return C_PUSH;
    if (!strcmp(cmd, "pop"))        return C_POP;
    if (!strcmp(cmd, "label"))      return C_LABEL;
    if (!strcmp(cmd, "goto"))       return C_GOTO;
    if (!strcmp(cmd, "if-goto"))    return C_IF;
    if (!strcmp(cmd, "function"))   return C_FUNCTION;
    if (!strcmp(cmd, "return"))     return C_RETURN;
    if (!strcmp(cmd, "call"))       return C_CALL;

    return C_INVALID;
}

void splitCmd(char *buf, char **cmd, char **arg1, char **arg2)
{
    char *delimiter = " \t\r\n";
    commandType type;

    *cmd = strtok(buf, delimiter);
    *arg1 = strtok(NULL, delimiter);
    *arg2 = strtok(NULL, delimiter);

    type = getCommandType(*cmd);

    if (type == C_ARITHMETIC) {
        *arg1 = *cmd;
        *arg2 = NULL;
    }
}