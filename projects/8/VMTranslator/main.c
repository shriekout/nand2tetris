#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

#include "config.h"
#include "parser.h"
#include "codeWriter.h"

#define EXTIN   ".vm"
#define EXTOUT  ".asm"

static void setFileName(char *);

char g_filename[BUF_MAX];

int main(int argc, char *argv[])
{
    // FILE *fin, *fout;
    // 
    struct stat st;
    DIR *dir;
    struct dirent *entry;
    char fnamein[BUF_MAX][BUF_MAX], fnameout[BUF_MAX];
    char *ext;
    int fileCount = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: VMTranslator source(.vm)\n");
        exit(1);
    }

    if (stat(argv[1], &st) == -1) {
        perror("stat");
        return 1;
    }

    if (S_ISDIR(st.st_mode)) {
        printf("Directory\n");
    } else if (S_ISREG(st.st_mode)) {
        strcpy(fnameout, argv[1]);
        ext = strrchr(fnameout, '.');

        if (ext == NULL ||strcmp(ext, EXTIN)) {
            fprintf(stderr, "Usage: VMTranslator source.vm\n");
            exit(1);
        }

        *ext = '\0';

        setFileName(fnameout);
    
        strcat(fnameout, EXTOUT);
        strcpy(fnamein[fileCount], argv[1]);
        fileCount++;
    } else {
        fprintf(stderr, "Unknown argv[1]\n");
        return 1;
    }

    for (int i = 0; i < fileCount; i++) {
        printf("%s\n", fnamein[i]);
    }

    return 0;
    
    // strcpy(fnamein, argv[1]);
    // strcpy(fnameout, argv[1]);

    // ext = strrchr(fnameout, '.');

    
    // }

    // *ext = '\0';

    // char *base = strrchr(fnameout, '/');

    // if (base != NULL)
    //     strcpy(g_filename, base+1);
    // else
    //     strcpy(g_filename, fnameout);
    
    // strcat(fnameout, EXTOUT);

    // if ((fin = fopen(fnamein, "r")) == NULL) {
    //     perror(fnamein);
    //     exit(1);
    // }

    // if ((fout = fopen(fnameout, "w")) == NULL) {
    //     perror(fnameout);
    //     exit(1);
    // }

    // Initialize(fout);

    // parser(fin, fout);

    // fclose(fin);
    // fclose(fout);
}

static void setFileName(char *fn)
{
    char *base = strrchr(fn, '/');

    if (base != NULL)
        strcpy(g_filename, base+1);
    else
        strcpy(g_filename, fn);
}