#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "jackTokenizer.h"

#define BUF_MAX 256

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

#define KEYWORD_COUNT (sizeof(keywordArr) / sizeof(keywordArr[0]))

int advance(FILE*, char*);
int isSymbol(char);
static tokenType getTokenType(char*);

void parser(FILE *fin, FILE *fout)
{
    char token[BUF_MAX];
    tokenType ty;
    const char *beginTag = "<tokens>";
    const char *endTag = "</tokens>";

    fprintf(fout, "%s\n", beginTag);

    while (advance(fin, token)) {
        ;
    }

    fprintf(fout, "%s", endTag);
}

int advance(FILE *fin, char *token)
{
    int c, d;
    int prev = 0;
    int pToken = 0;

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

            return 1;
            }
        } else {
            if (c == '"') {                 // STRING_CONST
                while ((c = fgetc(fin)) != EOF) {
                    if (c == '"')
                        break;
                    
                    token[pToken++] = c;
                }
            } else if (isSymbol(c)) {       // SYMBOL
                token[pToken++] = c;
            } else if (isdigit(c)) {        // INT_CONST
                token[pToken++] = c;

                while ((c = fgetc(fin)) != EOF) {
                    if (isdigit(c))
                        token[pToken++] = c;
                    else {
                        ungetc(c, fin);
                        break;
                    }
                }
            } else {                        // KEYWORD or IDENTIFIER
                token[pToken++] = c;

                while ((c = fgetc(fin)) != EOF) {
                    if (isalnum(c) || c == '_') {
                        token[pToken++] = c;
                    } else {
                        ungetc(c, fin);
                        break;
                    }
                }
            }

            token[pToken] = '\0';
            return 1;
        }
    }

    return 0;
}

int isSymbol(char c)
{
    return strchr("{}()[].,;+-*/&|<>=~", c) != NULL;
}

static tokenType getTokenType(char *buf)
{
    int i;

    for (i = 0; i < KEYWORD_COUNT; i++) {
        if (!strcmp(buf, keywordArr[i]))
            return KEYWORD;
    }
}