#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

typedef enum {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    C_INVALID
} commandType;

void parser(FILE*, FILE*);
int isBlank(const char*);
void removeComment(char*);
void ltrim(char*);
void rtrim(char*);
void trim(char*);
commandType getCommandType(const char*);
void splitCmd(char*, char**, char**, char**);

#endif