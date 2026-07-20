#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H

#include <stdio.h>

#include "types.h"

// Read one token.
tokenType advance(FILE*, char*);
// Push back one token.
void pushBack(tokenType, const char *);

#endif