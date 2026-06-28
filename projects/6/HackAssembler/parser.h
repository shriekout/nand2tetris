#ifndef PARSER_H
#define PARSER_H

typedef enum {
    A_INSTRUCTION,
    C_INSTRUCTION,
    L_INSTRUCTION
} InstructionType;

void removeWhitespace(char*);
int isBlank(const char*);
void removeComment(char*);
void extractSymbol( const char*, char*);

InstructionType getInstructionType(const char*);
int isNumber(const char*);
void extractComp(const char*, char*);
void extractDest(const char*, char*);
void extractJump(const char*, char*);

#endif