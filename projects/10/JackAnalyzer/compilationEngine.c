#include <stdio.h>
#include <string.h>

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

void compilationEngine(FILE *fin, FILE *fout)
{
    compileClass(fin, fout);
}

static void compileClass(FILE *fin, FILE *fout)
{
    tokenType type;
    char token[BUF_MAX];

    countSpace = 0;

    if (advance(fin, token) != KEYWORD || strcmp(token, "class")) {
        fprintf(stderr, "Unknown Entry point: %s\n", token);
        return;
    }

    printSpace(fout);
    fprintf(fout, "<class>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> class </keyword>\n");

    if (advance(fin, token) != IDENTIFIER) {
        fprintf(stderr, "Unknown class Name: %s\n", token);
        return;
    }
    printSpace(fout);
    fprintf(fout, "<identifier> %s </identifier>\n", token);

    if (advance(fin, token) != SYMBOL || strcmp(token, "{")) {
        fprintf(stderr, "Unknown the block start: %s\n", token);
        return;
    }
    xmlEscapeSymbol(token);
    printSpace(fout);
    fprintf(fout, "<symbol> %s </symbol>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD 
                && (!strcmp(token, "static")
                || (!strcmp(token, "field")))) {
            compileClassVarDec(fin, fout, token);
        } else if (type == KEYWORD 
                && (!strcmp(token, "constructor")
                || !strcmp(token, "method")
                || !strcmp(token, "function"))) {
            compileSubroutine(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
            break;
        }
    }

    countSpace--;
    printSpace(fout);
    fprintf(fout, "</class>\n");
}

static void compileClassVarDec(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<classVarDec>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> %s </keyword>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
        } else if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && strcmp(token, ";")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);

            countSpace--;
            printSpace(fout);
            fprintf(fout, "</classVarDec>\n");

            break;
        }
    }
}

static void compileSubroutine(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<subroutineDec>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> %s </keyword>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
        } else if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);    // (
            
            compileParameterList(fin, fout, token);

            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);    // )
        } else if (type == SYMBOL && !strcmp(token, "{")) {
            compileSubroutineBody(fin, fout, token);

            break;
        }
    }

    countSpace--;
    printSpace(fout);
    fprintf(fout, "</subroutineDec>\n");
}

static void compileParameterList(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<parameterList>\n");

    countSpace++;

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
        } else if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && strcmp(token, ")")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            countSpace--;

            printSpace(fout);
            fprintf(fout, "</parameterList>\n");

            break;
        }
    }
}

static void compileSubroutineBody(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<subroutineBody>\n");

    xmlEscapeSymbol(token);
    countSpace++;
    printSpace(fout);
    fprintf(fout, "<symbol> %s </symbol>\n", token);    // {

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD && !strcmp(token, "var")) {
            compileVarDec(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);    // }

            countSpace--;
            printSpace(fout);
            fprintf(fout, "</subroutineBody>\n");

            break;
        } else {
            pushBack(type, token);
            compileStatements(fin, fout, token);
        }
    }
}

static void compileVarDec(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<varDec>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> %s </keyword>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
        } else if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && strcmp(token, ";")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);

            countSpace--;
            printSpace(fout);
            fprintf(fout, "</varDec>\n");

            break;
        }
    }
}

static void compileStatements(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<statements>\n");

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

    printSpace(fout);
    fprintf(fout, "</statements>\n");
}

static void compileLet(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<letStatement>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> %s </keyword>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
        } else if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && !strcmp(token, "=")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);

            compileExpression(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
            break;
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);
        }
    }

    countSpace--;
    printSpace(fout);
    fprintf(fout, "</letStatement>\n");
    countSpace--;
}

static void compileIf(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<ifStatement>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> if </keyword>\n");

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, "(")) {
            printSpace(fout);
            fprintf(fout,"<symbol> ( </symbol>\n");

            compileExpression(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printSpace(fout);
            fprintf(fout, "<symbol> ) </symbol>\n");
        } else if (type == SYMBOL && !strcmp(token, "{")) {
            printSpace(fout);
            fprintf(fout, "<symbol> { </symbol>\n");

            compileStatements(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            printSpace(fout);
            fprintf(fout, "<symbol> } </symbol>\n");

            if ((type = advance(fin, token)) != TOKEN_EOF
                    && !strcmp(token, "else")) {
                pushBack(type, token);
            } else {
                pushBack(type, token);
                break;
            }
        } else if (type == KEYWORD && !strcmp(token, "else")) {
            printSpace(fout);
            fprintf(fout, "<keyword> else </keyword>\n");
        }
    }

    countSpace--;
    printSpace(fout);
    fprintf(fout, "</ifStatement>\n");

    countSpace--;
}

static void compileWhile(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<whileStatement>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> while </keyword>\n");

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, "(")) {
            printSpace(fout);
            fprintf(fout,"<symbol> ( </symbol>\n");

            compileExpression(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printSpace(fout);
            fprintf(fout, "<symbol> ) </symbol>\n");
        } else if (type == SYMBOL && !strcmp(token, "{")) {
            printSpace(fout);
            fprintf(fout, "<symbol> { </symbol>\n");

            compileStatements(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            printSpace(fout);
            fprintf(fout, "<symbol> } </symbol>\n");

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
    printSpace(fout);
    fprintf(fout, "</whileStatement>\n");

    countSpace--;
}

static void compileDo(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<doStatement>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> %s </keyword>\n", token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD) {
            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
        } else if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && strcmp(token, ";")) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);

            if (!strcmp(token, "(")) {
                compileExpressionList(fin, fout, token);
            }
        } else if (type == SYMBOL && !strcmp(token, ";")) {
                xmlEscapeSymbol(token);
                printSpace(fout);
                fprintf(fout, "<symbol> %s </symbol>\n", token);
                break;
        }
    }

    countSpace--;
    printSpace(fout);
    fprintf(fout, "</doStatement>\n");
    countSpace--;
}

static void compileReturn(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<returnStatement>\n");

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<keyword> return </keyword>\n");

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ";")) {
            printSpace(fout);
            fprintf(fout,"<symbol> ; </symbol>\n");
            
            break;
        } else {
            pushBack(type, token);
            compileExpression(fin, fout, token);
        }
    }

    countSpace--;
    printSpace(fout);
    fprintf(fout, "</returnStatement>\n");
    countSpace--;
}

static void compileExpression(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<expression>\n");

    compileTerm(fin, fout, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL 
                && (!strcmp(token, "+") 
                || !strcmp(token, "-") 
                || !strcmp(token, "*") 
                || !strcmp(token, "/") 
                ||!strcmp(token, "&")
                || !strcmp(token, "|") 
                || !strcmp(token, "<") 
                || !strcmp(token, ">") 
                || !strcmp(token, "="))) {
            xmlEscapeSymbol(token);
            printSpace(fout);
            fprintf(fout, "<symbol> %s </symbol>\n", token);

            compileTerm(fin, fout, token);
        } else {
            pushBack(type, token);
            break;
        }
    }

    printSpace(fout);
    fprintf(fout, "</expression>\n");
}

static void compileTerm(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    countSpace++;
    printSpace(fout);
    fprintf(fout, "<term>\n");

    if ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == IDENTIFIER) {
            countSpace++;

            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);

            if ((type = advance(fin, token)) != TOKEN_EOF) {
                if (type == SYMBOL && !strcmp(token, ".")) {
                    printSpace(fout);
                    fprintf(fout, "<symbol> . </symbol>\n");
                    compileSubroutineCall(fin, fout, token);
                } else {
                    pushBack(type, token);
                }
            }
            
            countSpace--;
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            pushBack(type, token);
        } else if (type == SYMBOL && !strcmp(token, ".")) {
            countSpace++;

            printSpace(fout);
            fprintf(fout, "<symbol> . </symbol>\n");
            compileSubroutineCall(fin, fout, token);

            countSpace--;
        } else if (type == KEYWORD) {
            countSpace++;

            printSpace(fout);
            fprintf(fout, "<keyword> %s </keyword>\n", token);
            
            countSpace--;
        } else if (type == STRING_CONST) {
            countSpace++;

            printSpace(fout);
            fprintf(fout, "<stringConstant> %s </stringConstant>\n", token);

            countSpace--;
        } else if (type == INT_CONST) {
            countSpace++;

            printSpace(fout);
            fprintf(fout, "<integerConstant> %s </integerConstant>\n", token);

            countSpace--;
        }
    }

    printSpace(fout);
    fprintf(fout, "</term>\n");
    countSpace--;
}

static void compileExpressionList(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    printSpace(fout);
    fprintf(fout, "<expressionList>\n");

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type != SYMBOL && strcmp(token, ")")) {
            countSpace++;
            pushBack(type, token);
            compileExpression(fin, fout, token);
            countSpace--;
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
            break;
        } else if (type == SYMBOL && !strcmp(token, ",")) {
            countSpace++;

            printSpace(fout);
            fprintf(fout, "<symbol> , </symbol>\n");

            countSpace--;
        } else if (type == IDENTIFIER) {
            countSpace++;

            pushBack(type, token);
            compileExpression(fin, fout, token);
            
            countSpace--;
        }
    }

    printSpace(fout);
    fprintf(fout, "</expressionList>\n");
}

static void compileSubroutineCall(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == IDENTIFIER) {
            printSpace(fout);
            fprintf(fout, "<identifier> %s </identifier>\n", token);
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            printSpace(fout);
            fprintf(fout, "<symbol> ( </symbol>\n");
            compileExpressionList(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            printSpace(fout);
            fprintf(fout, "<symbol> ) </symbol>\n");
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