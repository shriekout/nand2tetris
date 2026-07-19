#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jackTokenizer.h"
#include "compilationEngine.h"
#include "symbolTable.h"
#include "vmWriter.h"

static char className[MAX_BUF];
static char funcName[MAX_BUF];
static void compileClass(FILE*, FILE*);
static void compileClassVarDec(FILE*, FILE*, char*);
static void compileSubroutine(FILE*, FILE*, char*);
static void compileParameterList(FILE*, FILE*, char*);
static void compileSubroutineBody(FILE*, FILE*, char*);
static void compileVarDec(FILE*, FILE*, char*);
static void compileStatements(FILE*, FILE*, char*);
// static void compileLet(FILE*, FILE*, char*);
// static void compileIf(FILE*, FILE*, char*);
// static void compileWhile(FILE*, FILE*, char*);
static void compileDo(FILE*, FILE*, char*);
static void compileReturn(FILE*, FILE*, char*);
static void compileExpression(FILE*, FILE*, char*);
static void compileTerm(FILE*, FILE*, char*);
static int compileExpressionList(FILE*, FILE*, char*);
static void compileSubroutineCall(FILE*, FILE*, char*);

static int isOp(const char*);
static int isUnaryOp(const char*);

void compilationEngine(FILE *fin, FILE *fout)
{
    initializeClassTable();
    compileClass(fin, fout);
}

static void compileClass(FILE *fin, FILE *fout)
{
    tokenType type;
    char token[MAX_BUF];

    advance(fin, token);    // Read "class"

    advance(fin, token);    // Read className
    strcpy(className, token);

    advance(fin, token);    // Read "{"
    
    // Read class vars
    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD 
                && (!strcmp(token, "static") || !strcmp(token, "field"))) {
            compileClassVarDec(fin, fout, token);
        } else {
            pushBack(type, token);
            break;
        }
    }

    // Read constructor, method, function
    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD 
                && (!strcmp(token, "constructor") 
                    || !strcmp(token, "method")
                    || !strcmp(token, "function"))) {
            compileSubroutine(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            break;
        }
    }
}

static void compileClassVarDec(FILE *fin, FILE *fout, char *token)
{
//     tokenType type;
//     int isVar = !strcmp(token, "var");

//     if (isVar) {
//         openTag(fout, "varDec");
//     } else {
//         openTag(fout, "classVarDec");
//     }

//     countSpace++;
//     printToken(fout, KEYWORD, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == KEYWORD) {
//             printToken(fout, type, token);
//         } else if (type == IDENTIFIER) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && strcmp(token, ";")) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && !strcmp(token, ";")) {
//             printToken(fout, type, token);
//             countSpace--;

//             if (isVar) {
//                 closeTag(fout, "varDec");
//             } else {
//                 closeTag(fout, "classVarDec");
//             }

//             break;
//         }
//     }
}

static void compileSubroutine(FILE *fin, FILE *fout, char *token)
{
    initializeSubTable();

    if (!strcmp(token, "method")) {
        define("this", className, k_argument);
    }

    advance(fin, token);    // "void"? "int"? ...?

    advance(fin, token);    // Read function name. con? meth?
    strcpy(funcName, token);

    advance(fin, token);    // (

    compileParameterList(fin, fout, token);

    advance(fin, token);    // )

    compileSubroutineBody(fin, fout, token);
}

static void compileParameterList(FILE *fin, FILE *fout, char *token)
{
//     tokenType type;

//     openTag(fout, "parameterList");

//     countSpace++;

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == KEYWORD) {
//             printToken(fout, type, token);
//         } else if (type == IDENTIFIER) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && !strcmp(token, ",")) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && !strcmp(token, ")")) {
//             countSpace--;
//             closeTag(fout, "parameterList");
//             break;
//         }
//     }
}

static void compileSubroutineBody(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    char buf[MAX_BUF];

    advance(fin, token);    // {
    while ((type = advance(fin, token)) != EOF) {
        if (type == KEYWORD && !strcmp(token, "var")) {
            compileVarDec(fin, fout, token);
        } else {
            pushBack(type, token);
            break;
        }
    }

    sprintf(buf, "%s.%s", className, funcName);
    writeFunction(fout, buf, getVarCount(k_var));

    compileStatements(fin, fout, token);

    advance(fin, token);    // }
}

static void compileVarDec(FILE *fin, FILE *fout, char *token)
{
//     compileClassVarDec(fin, fout, token);
}

static void compileStatements(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD && !strcmp(token, "let")) {
            // compileLet(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "if")) {
            // compileIf(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "while")) {
            // compileWhile(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "do")) {
            compileDo(fin, fout, token);
        } else if (type == KEYWORD && !strcmp(token, "return")) {
            compileReturn(fin, fout, token);
        } else if (type == SYMBOL && !strcmp(token, "}")) {
            pushBack(type, token);
            break;
        }
    }
}

// static void compileLet(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     countSpace++;
//     openTag(fout, "letStatement");

//     countSpace++;
//     printToken(fout, KEYWORD, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == KEYWORD) {
//             printToken(fout, type, token);
//         } else if (type == IDENTIFIER) {
//             printToken(fout, type, token);

//             if ((type = advance(fin, token)) == SYMBOL &&
//                 !strcmp(token, "[")) {
//                 printToken(fout, type, token);
//                 compileExpression(fin, fout, token);

//                 if ((type = advance(fin, token)) == SYMBOL &&
//                     !strcmp(token, "]")) {
//                     printToken(fout, type, token);
//                 } else {
//                     perror("Expected ]");
//                     exit(1);
//                 }
//             } else {
//                 pushBack(type, token);
//             }
//         } else if (type == SYMBOL && !strcmp(token, "=")) {
//             printToken(fout, type, token);
//             compileExpression(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, ";")) {
//             printToken(fout, type, token);
//             break;
//         } else if (type == SYMBOL && !strcmp(token, ")")) {
//             printToken(fout, type, token);
//         }
//     }

//     countSpace--;
//     closeTag(fout, "letStatement");
//     countSpace--;
// }

// static void compileIf(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     countSpace++;
//     openTag(fout, "ifStatement");

//     countSpace++;
//     printToken(fout, KEYWORD, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == SYMBOL && !strcmp(token, "(")) {
//             printToken(fout, type, token);
//             compileExpression(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, ")")) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && !strcmp(token, "{")) {
//             printToken(fout, type, token);
//             compileStatements(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, "}")) {
//             printToken(fout, type, token);

//             if ((type = advance(fin, token)) != TOKEN_EOF
//                     && !strcmp(token, "else")) {
//                 pushBack(type, token);
//             } else {
//                 pushBack(type, token);
//                 break;
//             }
//         } else if (type == KEYWORD && !strcmp(token, "else")) {
//             printToken(fout, type, token);
//         }
//     }

//     countSpace--;
//     closeTag(fout, "ifStatement");
//     countSpace--;
// }

// static void compileWhile(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     countSpace++;
//     openTag(fout, "whileStatement");

//     countSpace++;
//     printToken(fout, KEYWORD, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == SYMBOL && !strcmp(token, "(")) {
//             printToken(fout, type, token);
//             compileExpression(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, ")")) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && !strcmp(token, "{")) {
//             printToken(fout, type, token);
//             compileStatements(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, "}")) {
//             printToken(fout, type, token);

//             if ((type = advance(fin, token)) != TOKEN_EOF
//                     && !strcmp(token, "else")) {
//                 pushBack(type, token);
//             } else {
//                 pushBack(type, token);
//                 break;
//             }
//         }
//     }

//     countSpace--;
//     closeTag(fout, "whileStatement");
//     countSpace--;
// }

static void compileDo(FILE *fin, FILE *fout, char *token)
{
    compileSubroutineCall(fin, fout, token);

    advance(fin, token);    // ;

    writePop(fout, TEMP, 0);
}

static void compileReturn(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    type = advance(fin, token);

    // return;
    if (type == SYMBOL && !strcmp(token, ";")) {
        writePush(fout, CONSTANT, 0);
        writeReturn(fout);
    }
//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == SYMBOL && !strcmp(token, ";")) {
//             printToken(fout, type, token);
//             break;
//         } else {
//             pushBack(type, token);
//             compileExpression(fin, fout, token);
//         }
//     }
}

static void compileExpression(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    arithmetic cmd;

    compileTerm(fin, fout, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && isOp(token)) {
            switch (*token) {
                case '+':   cmd = ADD;  break;

                case '-':   cmd = SUB;  break;

                case '*':   cmd = MUL;  break;

                case '/':   cmd = DIV;  break;

                case '=':   cmd = EQ;   break;

                case '>':   cmd = GT;   break;

                case '<':   cmd = LT;   break;

                case '&':   cmd = AND;  break;

                case '|':   cmd = OR;   break;
            }

            compileTerm(fin, fout, token);
            writeArithmetic(fout, cmd);
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
            break;
        }
    }
}

static void compileTerm(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

    if ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            compileExpression(fin, fout, token);
            advance(fin, token);    // )
        } else if (type == INT_CONST) {
            writePush(fout, CONSTANT, atoi(token));
        }
//         if (type == IDENTIFIER) {
//             countSpace++;
//             printToken(fout, type, token);

//             if ((type = advance(fin, token)) != TOKEN_EOF) {
//                 if (type == SYMBOL && !strcmp(token, ".")) {
//                     printToken(fout, type, token);
//                     compileSubroutineCall(fin, fout, token);
//                 } else if (type == SYMBOL && !strcmp(token, "[")) {
//                     printToken(fout, type, token);
//                     compileExpression(fin, fout, token);

//                     if ((type = advance(fin, token)) == SYMBOL &&
//                         !strcmp(token, "]")) {
//                         printToken(fout, type, token);
//                     } else {
//                         perror("Expected ]");
//                         exit(1);
//                     }
//                 } else {
//                     pushBack(type, token);
//                 }
//             }
            
//             countSpace--;
//         } else if (type == SYMBOL && !strcmp(token, ";")) {
//             pushBack(type, token);
//         } else if (type == SYMBOL && !strcmp(token, ".")) {
//             countSpace++;
//             printToken(fout, type, token);

//             compileSubroutineCall(fin, fout, token);
//             countSpace--;
//         } else if (type == KEYWORD) {
//             countSpace++;
//             printToken(fout, type, token);
//             countSpace--;
//         } else if (type == STRING_CONST) {
//             countSpace++;
//             printToken(fout, type, token);
//             countSpace--;
//         } else if (type == INT_CONST) {
//             countSpace++;
//             printToken(fout, type, token);
//             countSpace--;
//         } else if (type == SYMBOL && !strcmp(token, "(")) {
//             countSpace++;
//             printToken(fout, type, token);
//             compileExpression(fin, fout, token);

//             if ((type = advance(fin, token)) != TOKEN_EOF) {
//                 printToken(fout, type, token);
//                 countSpace--;
//                 closeTag(fout, "term");
//                 countSpace--;
//                 return;
//             }
//         } else if (type == SYMBOL && isUnaryOp(token)) {
//             countSpace++;
//             printToken(fout, type, token);
//             countSpace--;
//             compileTerm(fin, fout, token);
//         }
    }
}

static int compileExpressionList(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    int argCount = 0;

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
            break;
        }

        if (type == SYMBOL && !strcmp(token, ",")) {
            continue;
        }

        pushBack(type, token);
        compileExpression(fin, fout, token);
        argCount++;
    }

    return argCount;
}

static void compileSubroutineCall(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    char buf[MAX_BUF];
    int nArgs;

    advance(fin, token);    // identifier
    strcpy(buf, token);

    if ((type = advance(fin, token)) == SYMBOL && !strcmp(token, ".")) {
        strcat(buf, token);     // .

        advance(fin, token);    // identifier
        strcat(buf, token);
    }

    advance(fin, token);        // (

    nArgs = compileExpressionList(fin, fout, token);

    advance(fin, token);        // )

    writeCall(fout, buf, nArgs);
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