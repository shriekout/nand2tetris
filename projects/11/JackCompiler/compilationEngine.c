#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jackTokenizer.h"
#include "compilationEngine.h"
#include "symbolTable.h"

static char className[MAX_BUF];
static char funcName[MAX_BUF];

static void compileClass(FILE*, FILE*);
static void compileClassVarDec(FILE*, FILE*, char*);
static void compileSubroutine(FILE*, FILE*, char*);
static void compileParameterList(FILE*, FILE*, char*);
static void compileSubroutineBody(FILE*, FILE*, char*);
static void compileVarDec(FILE*, FILE*, char*);
// static void compileLet(FILE*, FILE*, char*);
// static void compileIf(FILE*, FILE*, char*);
// static void compileWhile(FILE*, FILE*, char*);
// static void compileDo(FILE*, FILE*, char*);
// static void compileReturn(FILE*, FILE*, char*);
// static void compileExpression(FILE*, FILE*, char*);
// static void compileTerm(FILE*, FILE*, char*);
// static void compileExpressionList(FILE*, FILE*, char*);
// static void compileSubroutineCall(FILE*, FILE*, char*);

static int isOp(const char*);
static int isUnaryOp(const char*);

void compilationEngine(FILE *fin, FILE *fout)
{
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
    int varCount = 0;

    while ((type = advance(fin, token)) != EOF) {
        if (type == KEYWORD && !strcmp(token, "var")) {
            compileVarDec(fin, fout, token);
            varCount++;
        } else {
            pushBack(type, token);
            break;
        }
    }

    // name = className + funcName;
    // writeFunction(name, nArgs);

    //     tokenType type;

//     openTag(fout, "statements");

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == KEYWORD && !strcmp(token, "let")) {
//             compileLet(fin, fout, token);
//         } else if (type == KEYWORD && !strcmp(token, "if")) {
//             compileIf(fin, fout, token);
//         } else if (type == KEYWORD && !strcmp(token, "while")) {
//             compileWhile(fin, fout, token);
//         } else if (type == KEYWORD && !strcmp(token, "do")) {
//             compileDo(fin, fout, token);
//         } else if (type == KEYWORD && !strcmp(token, "return")) {
//             compileReturn(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, "}")) {
//             pushBack(type, token);
//             break;
//         }
//     }

//     closeTag(fout, "statements");

}

static void compileVarDec(FILE *fin, FILE *fout, char *token)
{
//     compileClassVarDec(fin, fout, token);
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

// static void compileDo(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     countSpace++;
//     openTag(fout, "doStatement");

//     countSpace++;
//     printToken(fout, KEYWORD, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == KEYWORD) {
//             printToken(fout, type, token);
//         } else if (type == IDENTIFIER) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && strcmp(token, ";")) {
//             printToken(fout, type, token);

//             if (!strcmp(token, "(")) {
//                 compileExpressionList(fin, fout, token);
//             }
//         } else if (type == SYMBOL && !strcmp(token, ";")) {
//             printToken(fout, type, token);
//             break;
//         }
//     }

//     countSpace--;
//     closeTag(fout, "doStatement");
//     countSpace--;
// }

// static void compileReturn(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     countSpace++;
//     openTag(fout, "returnStatement");

//     countSpace++;
//     printToken(fout, KEYWORD, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == SYMBOL && !strcmp(token, ";")) {
//             printToken(fout, type, token);
//             break;
//         } else {
//             pushBack(type, token);
//             compileExpression(fin, fout, token);
//         }
//     }

//     countSpace--;
//     closeTag(fout, "returnStatement");
//     countSpace--;
// }

// static void compileExpression(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     openTag(fout, "expression");

//     compileTerm(fin, fout, token);

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == SYMBOL 
//                 && isOp(token)) {            
//             countSpace++;
//             printToken(fout, type, token);
//             countSpace--;

//             compileTerm(fin, fout, token);
//         } else {
//             pushBack(type, token);
//             break;
//         }
//     }

//     closeTag(fout, "expression");
// }

// static void compileTerm(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     countSpace++;
//     openTag(fout, "term");

//     if ((type = advance(fin, token)) != TOKEN_EOF) {
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
//     }

//     closeTag(fout, "term");
//     countSpace--;
// }

// static void compileExpressionList(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     openTag(fout, "expressionList");

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == SYMBOL && !strcmp(token, ")")) {
//             pushBack(type, token);
//             break;
//         }

//         if (type == SYMBOL && !strcmp(token, ",")) {
//             countSpace++;
//             printToken(fout, type, token);
//             countSpace--;
//             continue;
//         }

//         pushBack(type, token);
//         countSpace++;
//         compileExpression(fin, fout, token);
//         countSpace--;
//     }

//     closeTag(fout, "expressionList");
// }

// static void compileSubroutineCall(FILE *fin, FILE *fout, char *token)
// {
//     tokenType type;

//     while ((type = advance(fin, token)) != TOKEN_EOF) {
//         if (type == IDENTIFIER) {
//             printToken(fout, type, token);
//         } else if (type == SYMBOL && !strcmp(token, "(")) {
//             printToken(fout, type, token);
//             compileExpressionList(fin, fout, token);
//         } else if (type == SYMBOL && !strcmp(token, ")")) {
//             printToken(fout, type, token);
//             break;
//         } 
//     }
// }

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