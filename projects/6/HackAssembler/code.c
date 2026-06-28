#include "code.h"

void getBinary(const int addr, char *tmp)
{
    for (int i = 15; i >= 0; i--) {
        *tmp++ = ((addr >> i) & 1) + '0';
    }

    *tmp = '\0';
}