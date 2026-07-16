#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jackTokenizer.h"
#include "compilationEngine.h"

static int countSpace = 0;

static void compileClass(FILE*, FILE*);
static void compileClassVarDec(FILE*, FILE*, char*);
static void compileSubroutine(FILE*, FILE*, char*);
static void compileParameterList(FILE*, FILE*, char*);
static void compileSubroutineBody(FILE*, FILE*, char*);
static void compileVarDec(FILE*, FILE*, char*);
static void compileStatements(FILE*, FILE*, char*);
static void compileLet(FILE*, FILE*, char*);
static void compileIf(FILE*, FILE*, char*);
static void compileWhile(FILE*, FILE*, char*);
static void compileDo(FILE*, FILE*, char*);
static void compileReturn(FILE*, FILE*, char*);
static void compileExpression(FILE*, FILE*, char*);
static void compileTerm(FILE*, FILE*, char*);
static void compileExpressionList(FILE*, FILE*, char*);
static void compileSubroutineCall(FILE*, FILE*, char*);

static void xmlEscapeSymbol(char*);
static void printSpace(FILE*);
static void printToken(FILE*, tokenType, char*);
static void openTag(FILE*, const char*);
static void closeTag(FILE*, const char*);
static int isOp(const char*);
static int isUnaryOp(const char*);

void compilationEngine(FILE *fin, FILE *fout)
{
    compileClass(fin, fout);
}

static void compileClass(FILE *fin, FILE *fout)
{
    tokenType type;
    char token[BUF_MAX];

    countSpace = 0;

    if ((type = advance(fin, token)) != KEYWORD || strcmp(token, "class")) {
        fprintf(stderr, "Unknown Entry point: %s\n", token);
        return;
    }

    openTag(fout, "class");

    countSpace++;
    printToken(fout, type, token);

    if ((type = advance(fin, token)) != IDENTIFIER) {
        fprintf(stderr, "Unknown class Name: %s\n", token);
        return;
    }

    printToken(fout, type, token);

    if ((type = advance(fin, token)) != SYMBOL || strcmp(token, "{")) {
        fprintf(stderr, "Unknown the block start: %s\n", token);
        return;
    }
    
    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD 
                && (!strcmp(token, "static")
                || !strcmp(token, "field"))) {
            compileClassVarDec(fin, fout, token);
        } else if (type == KEYWORD 
                && (!strcmp(token, "constructor")
                || !strcmp(token, "method")
                || !strcmp(token, "function"))) {
            compileSubroutine(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            printToken(fout, type, token);
            break;
        }
    }

    countSpace--;
    closeTag(fout, "class");
}

static void compileClassVarDec(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    int isVar = !strcmp(token, "var");

    if (isVar) {
        openTag(fout, "varDec");
    } else {
        openTag(fout, "classVarDec");
    }

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printToken(fout, type, token);
        } else if (type == IDENTIFIER) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && strcmp(token, ";")) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            printToken(fout, type, token);
            countSpace--;

            if (isVar) {
                closeTag(fout, "varDec");
            } else {
                closeTag(fout, "classVarDec");
            }

            break;
        }
    }
}

static void compileSubroutine(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    openTag(fout, "subroutineDec");

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printToken(fout, type, token);
        } else if (type == IDENTIFIER) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            printToken(fout, type, token);
            compileParameterList(fin, fout, token);
            printToken(fout, type, token);      // )
        } else if (type == SYMBOL && !strcmp(token, "{")) {
            compileSubroutineBody(fin, fout, token);
            break;
        }
    }

    countSpace--;
    closeTag(fout, "subroutineDec");
}

static void compileParameterList(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    openTag(fout, "parameterList");

    countSpace++;

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printToken(fout, type, token);
        } else if (type == IDENTIFIER) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, ",")) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            countSpace--;
            closeTag(fout, "parameterList");
            break;
        }
    }
}

static void compileSubroutineBody(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    openTag(fout, "subroutineBody");

    countSpace++;
    printToken(fout, SYMBOL, token);    // {

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD && !strcmp(token, "var")) {
            compileVarDec(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            printToken(fout, type, token);

            countSpace--;
            closeTag(fout, "subroutineBody");

            break;
        } else {
            pushBack(type, token);
            compileStatements(fin, fout, token);
        }
    }
}

static void compileVarDec(FILE *fin, FILE *fout, char *token)
{
    compileClassVarDec(fin, fout, token);
}

static void compileStatements(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    openTag(fout, "statements");

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD && !strcmp(token, "let")) {
            compileLet(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "if")) {
            compileIf(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "while")) {
            compileWhile(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "do")) {
            compileDo(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "return")) {
            compileReturn(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            pushBack(type, token);
            break;
        }
    }

    closeTag(fout, "statements");
}

static void compileLet(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    openTag(fout, "letStatement");

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printToken(fout, type, token);
        } else if (type == IDENTIFIER) {
            printToken(fout, type, token);

            if ((type = advance(fin, token)) == SYMBOL &&
                !strcmp(token, "[")) {
                printToken(fout, type, token);
                compileExpression(fin, fout, token);

                if ((type = advance(fin, token)) == SYMBOL &&
                    !strcmp(token, "]")) {
                    printToken(fout, type, token);
                } else {
                    perror("Expected ]");
                    exit(1);
                }
            } else {
                pushBack(type, token);
            }
        } else if (type == SYMBOL && !strcmp(token, "=")) {
            printToken(fout, type, token);
            compileExpression(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            printToken(fout, type, token);
            break;
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printToken(fout, type, token);
        }
    }

    countSpace--;
    closeTag(fout, "letStatement");
    countSpace--;
}

static void compileIf(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    openTag(fout, "ifStatement");

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, "(")) {
            printToken(fout, type, token);
            compileExpression(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, "{")) {
            printToken(fout, type, token);
            compileStatements(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            printToken(fout, type, token);

            if ((type = advance(fin, token)) != TOKEN_EOF
                    && !strcmp(token, "else")) {
                pushBack(type, token);
            } else {
                pushBack(type, token);
                break;
            }
        } else if (type == KEYWORD && !strcmp(token, "else")) {
            printToken(fout, type, token);
        }
    }

    countSpace--;
    closeTag(fout, "ifStatement");
    countSpace--;
}

static void compileWhile(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    openTag(fout, "whileStatement");

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, "(")) {
            printToken(fout, type, token);
            compileExpression(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, "{")) {
            printToken(fout, type, token);
            compileStatements(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            printToken(fout, type, token);

            if ((type = advance(fin, token)) != TOKEN_EOF
                    && !strcmp(token, "else")) {
                pushBack(type, token);
            } else {
                pushBack(type, token);
                break;
            }
        }
    }

    countSpace--;
    closeTag(fout, "whileStatement");
    countSpace--;
}

static void compileDo(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    openTag(fout, "doStatement");

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printToken(fout, type, token);
        } else if (type == IDENTIFIER) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && strcmp(token, ";")) {
            printToken(fout, type, token);

            if (!strcmp(token, "(")) {
                compileExpressionList(fin, fout, token);
            }
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            printToken(fout, type, token);
            break;
        }
    }

    countSpace--;
    closeTag(fout, "doStatement");
    countSpace--;
}

static void compileReturn(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    openTag(fout, "returnStatement");

    countSpace++;
    printToken(fout, KEYWORD, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ";")) {
            printToken(fout, type, token);
            break;
        } else {
            pushBack(type, token);
            compileExpression(fin, fout, token);
        }
    }

    countSpace--;
    closeTag(fout, "returnStatement");
    countSpace--;
}

static void compileExpression(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    openTag(fout, "expression");

    compileTerm(fin, fout, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL 
                && isOp(token)) {            
            countSpace++;
            printToken(fout, type, token);
            countSpace--;

            compileTerm(fin, fout, token);
        } else {
            pushBack(type, token);
            break;
        }
    }

    closeTag(fout, "expression");
}

static void compileTerm(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    openTag(fout, "term");

    if ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == IDENTIFIER) {
            countSpace++;
            printToken(fout, type, token);

            if ((type = advance(fin, token)) != TOKEN_EOF) {
                if (type == SYMBOL && !strcmp(token, ".")) {
                    printToken(fout, type, token);
                    compileSubroutineCall(fin, fout, token);
                } else if (type == SYMBOL && !strcmp(token, "[")) {
                    printToken(fout, type, token);
                    compileExpression(fin, fout, token);

                    if ((type = advance(fin, token)) == SYMBOL &&
                        !strcmp(token, "]")) {
                        printToken(fout, type, token);
                    } else {
                        perror("Expected ]");
                        exit(1);
                    }
                } else {
                    pushBack(type, token);
                }
            }
            
            countSpace--;
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            pushBack(type, token);
        } else if (type == SYMBOL && !strcmp(token, ".")) {
            countSpace++;
            printToken(fout, type, token);

            compileSubroutineCall(fin, fout, token);
            countSpace--;
        } else if (type == KEYWORD) {
            countSpace++;
            printToken(fout, type, token);
            countSpace--;
        } else if (type == STRING_CONST) {
            countSpace++;
            printToken(fout, type, token);
            countSpace--;
        } else if (type == INT_CONST) {
            countSpace++;
            printToken(fout, type, token);
            countSpace--;
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            countSpace++;
            printToken(fout, type, token);
            compileExpression(fin, fout, token);

            if ((type = advance(fin, token)) != TOKEN_EOF) {
                printToken(fout, type, token);
                countSpace--;
                closeTag(fout, "term");
                countSpace--;
                return;
            }
        } else if (type == SYMBOL && isUnaryOp(token)) {
            countSpace++;
            printToken(fout, type, token);
            countSpace--;
            compileTerm(fin, fout, token);
        }
    }

    closeTag(fout, "term");
    countSpace--;
}

static void compileExpressionList(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    openTag(fout, "expressionList");

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
            break;
        }

        if (type == SYMBOL && !strcmp(token, ",")) {
            countSpace++;
            printToken(fout, type, token);
            countSpace--;
            continue;
        }

        pushBack(type, token);
        countSpace++;
        compileExpression(fin, fout, token);
        countSpace--;
    }

    closeTag(fout, "expressionList");
}

static void compileSubroutineCall(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == IDENTIFIER) {
            printToken(fout, type, token);
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            printToken(fout, type, token);
            compileExpressionList(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printToken(fout, type, token);
            break;
        } 
    }
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

static void printSpace(FILE *fout)
{
    for (int i = 0; i < countSpace; i++)
        fprintf(fout, "  ");
}

static void printToken(FILE *fout, tokenType type, char *token)
{
    if (type == SYMBOL)
        xmlEscapeSymbol(token);

    printSpace(fout);

    switch (type) {
        case KEYWORD:
            fprintf(fout, "<keyword> %s </keyword>\n", token);
            break;

        case SYMBOL:
            fprintf(fout, "<symbol> %s </symbol>\n", token);
            break;

        case IDENTIFIER:
            fprintf(fout, "<identifier> %s </identifier>\n", token);
            break;
        
        case INT_CONST:
            fprintf(fout, "<integerConstant> %s </integerConstant>\n", token);
            break;

        case STRING_CONST:
            fprintf(fout, "<stringConstant> %s </stringConstant>\n", token);
            break;

        default:
            break;
    }
}

static void openTag(FILE *fout, const char *tag)
{
    printSpace(fout);
    fprintf(fout, "<%s>\n", tag);
}

static void closeTag(FILE *fout, const char *tag)
{
    printSpace(fout);
    fprintf(fout, "</%s>\n", tag);
}

static int isOp(const char *token)
{
    return !strcmp(token, "+") 
            || !strcmp(token, "-") 
            || !strcmp(token, "*") 
            || !strcmp(token, "/") 
            || !strcmp(token, "&")
            || !strcmp(token, "|") 
            || !strcmp(token, "<") 
            || !strcmp(token, ">") 
            || !strcmp(token, "=");
}

static int isUnaryOp(const char *token)
{
    return !strcmp(token, "-") || !strcmp(token, "~");
}