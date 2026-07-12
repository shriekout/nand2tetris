#include <stdio.h>
#include <string.h>

#include "jackTokenizer.h"
#include "compilationEngine.h"

static int countTab = 0;

static void compileClass(FILE*, FILE*);
static void compileSubroutine(FILE*, FILE*, char*);
static void xmlEscapeSymbol(char*);
static void printTab(FILE*);

void compilationEngine(FILE *fin, FILE *fout)
{
    compileClass(fin, fout);
}

static void compileClass(FILE *fin, FILE *fout)
{
    tokenType type;
    char token[BUF_MAX];

    if (advance(fin, token) != KEYWORD || strcmp(token, "class")) {
        fprintf(stderr, "Unknown Entry point: %s\n", token);
        return;
    }

    printTab(fout);
    fprintf(fout, "<class>\n");

    countTab++;
    printTab(fout);
    fprintf(fout, "<keyword> class </keyword>\n");

    if (advance(fin, token) != IDENTIFIER) {
        fprintf(stderr, "Unknown class Name: %s\n", token);
        return;
    }
    printTab(fout);
    fprintf(fout, "<identifier> %s </identifier>\n", token);

    if (advance(fin, token) != SYMBOL || strcmp(token, "{")) {
        fprintf(stderr, "Unknown the block start: %s\n", token);
        return;
    }
    xmlEscapeSymbol(token);
    printTab(fout);
    fprintf(fout, "<symbol> %s </symbol>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD 
            && (!strcmp(token, "constructor")
            || !strcmp(token, "method")
            || !strcmp(token, "function"))) {
            compileSubroutine(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            xmlEscapeSymbol(token);
            printTab(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
            break;
        }
    }

    countTab--;
    printTab(fout);
    fprintf(fout, "</class>\n");
}

static void compileSubroutine(FILE *fin, FILE *fout, char *token)
{
    
}

static void xmlEscapeSymbol(char *symbol)
{
    switch (symbol[0]) {
        case '<':
            strcpy(symbol, "&lt;");
            break;

        case '>':
            strcpy(symbol, "&gt;");
            break;

        case '&':
            strcpy(symbol, "&amp;");
            break;
        default:
            break;
    }
}

static void printTab(FILE *fout)
{
    for (int i = 0; i < countTab; i++)
        fputc('\t', fout);
}