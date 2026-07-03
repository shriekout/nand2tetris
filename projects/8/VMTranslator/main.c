#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "parser.h"
#include "codeWriter.h"

#define EXTIN   ".vm"
#define EXTOUT  ".asm"

char g_filename[BUF_MAX];

int main(int argc, char *argv[])
{
    FILE *fin, *fout;
    char fnamein[BUF_MAX], fnameout[BUF_MAX];
    char *ext;

    if (argc != 2) {
        fprintf(stderr, "Usage: VMTranslator source.vm\n");
        exit(1);
    }

    strcpy(fnamein, argv[1]);
    strcpy(fnameout, argv[1]);

    ext = strrchr(fnameout, '.');

    if (ext == NULL ||strcmp(ext, EXTIN)) {
        fprintf(stderr, "Usage: VMTranslator source.vm\n");
        exit(1);
    }

    *ext = '\0';

    char *base = strrchr(fnameout, '/');

    if (base != NULL)
        strcpy(g_filename, base+1);
    else
        strcpy(g_filename, fnameout);
    
    strcat(fnameout, EXTOUT);

    if ((fin = fopen(fnamein, "r")) == NULL) {
        perror(fnamein);
        exit(1);
    }

    if ((fout = fopen(fnameout, "w")) == NULL) {
        perror(fnameout);
        exit(1);
    }

    Initialize(fout);

    parser(fin, fout);

    fclose(fin);
    fclose(fout);
}