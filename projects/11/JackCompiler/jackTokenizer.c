#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "jackTokenizer.h"

static const char *keywordArr[] = {
    "class",
    "method",
    "function",
    "constructor",
    "int",
    "boolean",
    "char",
    "void",
    "var",
    "static",
    "field",
    "let",
    "do",
    "if",
    "else",
    "while",
    "return",
    "true",
    "false",
    "null",
    "this"
};

static tokenType prevType;
static char prevToken[MAX_BUF];
static int hasPrev = 0;

#define KEYWORD_COUNT (sizeof(keywordArr) / sizeof(keywordArr[0]))

static int isSymbol(char);

tokenType advance(FILE *fin, char *token)
{
    int c, d;
    int prev = 0;
    int pToken = 0;

    if (hasPrev) {
        hasPrev = 0;
        strcpy(token, prevToken);
        return prevType;
    }

    while ((c = fgetc(fin)) != EOF) {
        if (isspace(c)) {
            continue;
        } else if (c == '/') {
            if ((d = fgetc(fin)) == '/') {  // // comment
                while ((d = fgetc(fin)) != EOF) {
                    if (d == '\n')
                        break;
                }
            } else if (d == '*') {  // /* comment
                prev = 0;

                while ((d = fgetc(fin)) != EOF) {
                    if (prev == '*' && d == '/')
                        break;

                    prev = d;
                }
            } else {
            token[pToken++] = '/';
            token[pToken] = '\0';
            ungetc(d, fin);

            return SYMBOL;
            }
        } else {
            if (c == '"') {                 // STRING_CONST
                while ((c = fgetc(fin)) != EOF) {
                    if (c == '"') {
                        token[pToken] = '\0';
                        return STRING_CONST;
                    }
                    
                    token[pToken++] = c;
                }
            } else if (isSymbol(c)) {       // SYMBOL
                token[pToken++] = c;
                token[pToken] = '\0';

                return SYMBOL;
            } else if (isdigit(c)) {        // INT_CONST
                token[pToken++] = c;

                while ((c = fgetc(fin)) != EOF) {
                    if (isdigit(c))
                        token[pToken++] = c;
                    else {
                        ungetc(c, fin);
                        token[pToken] = '\0';
                        return INT_CONST;
                    }
                }

                token[pToken] = '\0';
                return INT_CONST;
            } else {                        // KEYWORD or IDENTIFIER
                token[pToken++] = c;

                while ((c = fgetc(fin)) != EOF) {
                    if (isalnum(c) || c == '_') {
                        token[pToken++] = c;
                    } else {
                        ungetc(c, fin);
                        token[pToken] = '\0';

                        for (int i = 0; i < KEYWORD_COUNT; i++) {
                            if (!strcmp(token, keywordArr[i]))
                                return KEYWORD;
                        }
                        
                        return IDENTIFIER;
                    }
                }

                token[pToken] = '\0';
                
                for (int i = 0; i < KEYWORD_COUNT; i++) {
                    if (!strcmp(token, keywordArr[i]))
                        return KEYWORD;
                    }
                
                return IDENTIFIER;
            }
        }
    }

    return TOKEN_EOF;
}

void pushBack(tokenType type, const char *token)
{
    if (hasPrev) {
        fprintf(stderr, "pushBack(): token already exists\n");
        return;
    }

    strcpy(prevToken, token);
    prevType = type;
    hasPrev = 1;
}


static int isSymbol(char c)
{
    return strchr("{}()[].,;+-*/&|<>=~", c) != NULL;
}