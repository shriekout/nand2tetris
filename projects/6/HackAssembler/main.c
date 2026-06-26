#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include "config.h"

int main(int argc, char *argv[])
{
    char *fn;
    char infile[BUF_MAX], outfile[BUF_MAX];
    const char *inext = "asm", *outext = "hack";
    FILE *infp, *outfp;

    if (argc != 2) {
        fprintf(stderr, "Usage: ./HackAssembler source.asm\n");
        return 1;
    }

    if (strlen(argv[1]) >= BUF_MAX) {
        fprintf(stderr, "File name is too long.\n");
        return 1;
    }

    strcpy(infile, argv[1]);
    strcpy(outfile, argv[1]);

    fn = strrchr(outfile, '.');

    if (fn != NULL) {
        if (strcmp(fn+1, inext)) {
            fprintf(stderr, "Usage: ./HackAssembler source.asm\n");
            return 1;
        }

        *(fn+1) = '\0';
        strcat(outfile, outext);
    } else {
        fprintf(stderr, "Usage: ./HackAssembler source.asm\n");
        return 1;
    }

    initSymbolTable();
    containsSymbol("aaa");

    return 0;
}