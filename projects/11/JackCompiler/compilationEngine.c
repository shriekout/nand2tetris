#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "jackTokenizer.h"
#include "compilationEngine.h"
#include "symbolTable.h"
#include "vmWriter.h"

static char className[MAX_BUF];
static char funcName[MAX_BUF];
static int loopNum = 0;
static int isConstructor = 0;
static int isMethod = 0;

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
static int compileExpressionList(FILE*, FILE*, char*);
static void compileSubroutineCall(FILE*, FILE*, char*);

static int isOp(const char*);
static int isUnaryOp(const char*);
static segment varKindToSegment(varKind);

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
    tokenType type;
    varKind k;
    char t[MAX_BUF];

    if (!strcmp(token, "static"))
        k = k_static;
    else
        k = k_field;

    advance(fin, token);    // Read type
    strcpy(t, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ",")) {
            continue;
        } else if (type == IDENTIFIER) {
            define(token, t, k);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            break;
        }
    }
}

static void compileSubroutine(FILE *fin, FILE *fout, char *token)
{
    isConstructor = 0;
    isMethod = 0;

    initializeSubTable();

    if (!strcmp(token, "method")) {
        define("this", className, k_argument);
        isMethod = 1;
    } else if (!strcmp(token, "constructor")) {
        isConstructor = 1;
    }

    advance(fin, token);    // "void"? "int"? ...?

    advance(fin, token);    // Read function(constructor? method?) name
    strcpy(funcName, token);

    advance(fin, token);    // (

    compileParameterList(fin, fout, token);

    advance(fin, token);    // )

    compileSubroutineBody(fin, fout, token);
}

static void compileParameterList(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    char name[MAX_BUF];
    char t[MAX_BUF];

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ",")) {
            continue;
        } else if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
            break;
        } else {
            strcpy(t, token);

            advance(fin, token);
            strcpy(name, token);

            define(name, t, k_argument);
        }
    }
}

static void compileSubroutineBody(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    char buf[MAX_BUF];

    advance(fin, token);    // {
    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == KEYWORD && !strcmp(token, "var")) {
            compileVarDec(fin, fout, token);
        } else {
            pushBack(type, token);
            break;
        }
    }

    sprintf(buf, "%s.%s", className, funcName);
    writeFunction(fout, buf, getVarCount(k_var));

    if (isConstructor) {
        writePush(fout, CONSTANT, getVarCount(k_field));
        writeCall(fout, "Memory.alloc", 1);
        writePop(fout, POINTER, 0);
    } else if (isMethod) {
        writePush(fout, ARGUMENT, 0);
        writePop(fout, POINTER, 0);
    }

    compileStatements(fin, fout, token);

    advance(fin, token);    // }
}

static void compileVarDec(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    char t[MAX_BUF];

    advance(fin, token);
    strcpy(t, token);

    while ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ",")) {
            continue;
        } else if (type == IDENTIFIER) {
            define(token, t, k_var);
        } else if (type == SYMBOL && !strcmp(token, ";")) {
            break;
        }
    }
}

static void compileStatements(FILE *fin, FILE *fout, char *token)
{
    tokenType type;

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
}

static void compileLet(FILE *fin, FILE *fout, char *token)
{
    char varName[MAX_BUF];

    advance(fin, token);    // Read variable name
    strcpy(varName, token);

    advance(fin, token);    // Read "="

    compileExpression(fin, fout, token);

    writePop(fout, varKindToSegment(kindOf(varName)), indexOf(varName));
}

static void compileIf(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    char buf1[MAX_BUF], buf2[MAX_BUF];
    int n = loopNum++;

    sprintf(buf1, "IF_FALSE%d", n);
    sprintf(buf2, "IF_END%d", n);

    advance(fin, token);    // Read "("

    compileExpression(fin, fout, token);
    writeArithmetic(fout, NOT);

    writeIf(fout, buf1);

    advance(fin, token);    // Read ")"

    advance(fin, token);    // Read "{"

    compileStatements(fin, fout, token);

    advance(fin, token);    // Read "}"

    writeGoto(fout, buf2);
    writeLabel(fout, buf1);

    type = advance(fin, token);

    if (type == KEYWORD && !strcmp(token, "else")) {
        advance(fin, token);    // Read "{"

        compileStatements(fin, fout, token);

        advance(fin, token);    // Read "}"
    } else {
        pushBack(type, token);
    }

    writeLabel(fout, buf2);
}

static void compileWhile(FILE *fin, FILE *fout, char *token)
{
    char buf1[MAX_BUF], buf2[MAX_BUF];
    int n = loopNum++;

    sprintf(buf1, "WHILE_EXP%d", n);
    sprintf(buf2, "WHILE_END%d", n);
    

    advance(fin, token);    // Read "("

    writeLabel(fout, buf1);

    compileExpression(fin, fout, token);

    writeArithmetic(fout, NOT);
    writeIf(fout, buf2);

    advance(fin, token);    // Read ")"


    advance(fin, token);    // Read "{"

    compileStatements(fin, fout, token);

    advance(fin, token);    // Read "}"

    writeGoto(fout, buf1);
    writeLabel(fout, buf2);
}

static void compileDo(FILE *fin, FILE *fout, char *token)
{
    advance(fin, token);

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
    } else {
        pushBack(type, token);
        compileExpression(fin, fout, token);
    }

    writeReturn(fout);
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
        } else {
            pushBack(type, token);
            break;
        }
    }
}

static void compileTerm(FILE *fin, FILE *fout, char *token)
{
    tokenType type;
    arithmetic op;
    char buf[MAX_BUF];
    char c;

    if ((type = advance(fin, token)) != TOKEN_EOF) {
        if (type == SYMBOL && !strcmp(token, ")")) {
            pushBack(type, token);
        } else if (type == SYMBOL && !strcmp(token, "(")) {
            compileExpression(fin, fout, token);
            advance(fin, token);    // )
        } else if (type == INT_CONST) {
            writePush(fout, CONSTANT, atoi(token));
        } else if (type == SYMBOL && isUnaryOp(token)) {
            if (!strcmp(token, "~")) {
                op = NOT;
            } else if (!strcmp(token, "-")) {
                op = NEG;
            }
            
            compileTerm(fin, fout, token);
            writeArithmetic(fout, op);
        } else if (type == STRING_CONST) {
            int len = strlen(token);
            char *p = token;

            while (*p != '\0') {

                p++;
            }
        } else if (type == IDENTIFIER) {
            strcpy(buf, token);

            type = advance(fin, token);

            if (!strcmp(token, ".")) {
                pushBack(type, token);
                strcpy(token, buf);
                compileSubroutineCall(fin, fout, token);
            } else if (!strcmp(token, "[")) {

            } else {
                pushBack(type, token);
                writePush(fout, varKindToSegment(kindOf(buf)), indexOf(buf));
            }
        } else if (type == KEYWORD && !strcmp(token, "true")) {
            writePush(fout, CONSTANT, 0);
            writeArithmetic(fout, NOT);
        } else if (type == KEYWORD && !strcmp(token, "false")) {
            writePush(fout, CONSTANT, 0);
        } else if (type == KEYWORD && !strcmp(token, "this")) {
            writePush(fout, POINTER, 0);
        }
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
        } else if (type == SYMBOL && !strcmp(token, ",")) {
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
    char fullname[MAX_BUF];
    int nArgs = 0;
    int isMethod = 0;

    strcpy(buf, token);

    if (indexOf(buf) != -1) {
        writePush(fout, varKindToSegment(kindOf(buf)), indexOf(buf));
        strcpy(buf, typeOf(buf));
        isMethod = 1;
    }

    if ((type = advance(fin, token)) == SYMBOL && !strcmp(token, ".")) {
        strcat(buf, token);     // .

        advance(fin, token);    // identifier
        strcat(buf, token);
    } else {
        // token == "("
        pushBack(type, token);

        writePush(fout, POINTER, 0);
        sprintf(fullname, "%s.%s", className, buf);
        strcpy(buf, fullname);
        nArgs = 1;
    }

    advance(fin, token);        // (

    nArgs += compileExpressionList(fin, fout, token);
    nArgs += isMethod;

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

static segment varKindToSegment(varKind kind)
{
    switch (kind) {
        case k_none:        return -1;
        case k_static:      return STATIC;
        case k_field:       return THIS;
        case k_argument:    return ARGUMENT;
        case k_var:         return LOCAL;
    }

    return -1;
}