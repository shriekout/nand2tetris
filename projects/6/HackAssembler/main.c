#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "symboltable.h"
#include "config.h"
#include "assembler.h"

int main(int argc, char *argv[])
{
    char *fn;
    char infile[BUF_MAX], outfile[BUF_MAX];
    const char *inext = "asm", *outext = ".hack";
    FILE *fin, *fout;

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

        *fn = '\0';
        strcat(outfile, outext);
    } else {
        fprintf(stderr, "Usage: ./HackAssembler source.asm\n");
        return 1;
    }

    initSymbolTable();

    fin = fopen(infile, "r");
    if (fin == NULL) {
        perror(infile);
        return EXIT_FAILURE;
    }

    fout = fopen(outfile, "w");
    if (fout == NULL) {
        perror(outfile);
        return EXIT_FAILURE;
    }

    firstPass(fin);

    rewind(fin);
    
    secondPass(fin, fout);

    fclose(fin);
    fclose(fout);

    return 0;
}