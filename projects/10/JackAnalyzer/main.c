#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>

#include "jackTokenizer.h"

typedef enum {
    READ_FAIL,
    WRITE_FAIL,
    SUCCESS
} fileIO;

static int isJack(const char*);
static void changeExt(char*);
static fileIO fileProcess(const char*, const char*);

int main(int argc, char *argv[])
{
    struct stat st;
    DIR *dir;
    struct dirent *entry;
    char infn[PATH_MAX], outfn[PATH_MAX];

    if (argc != 2) {
        fprintf(stderr, "Usage: JackAnalyzer source.jack\n");
        return 1;
    }

    if (stat(argv[1], &st) == -1) {
        perror("stat");
        return 1;
    }

    if (S_ISDIR(st.st_mode)) {
        if ((dir = opendir(argv[1])) == NULL) {
            perror(argv[1]);
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            // ignore "." ".."
            if (!strcmp(entry->d_name, ".")
                || !strcmp(entry->d_name, ".."))
                continue;

            if (isJack(entry->d_name)) {
                if (argv[1][strlen(argv[1])-1] == '/') {
                    strcpy(infn, argv[1]);
                } else {
                    strcpy(infn, argv[1]);
                    strcat(infn, "/");
                }

                strcpy(outfn, infn);
                strcat(infn, entry->d_name);
                strcat(outfn, entry->d_name);
                changeExt(outfn);

                fileProcess(infn, outfn);
            }
        }

        closedir(dir);
    } else if (S_ISREG(st.st_mode)) {
        if (!isJack(argv[1])) {
            fprintf(stderr, "Usage: JackAnalyzer source.jack\n");
            return 1;
        }

        strcpy(infn, argv[1]);
        strcpy(outfn, argv[1]);
        changeExt(outfn);

        fileProcess(infn, outfn);
    } else {
        fprintf(stderr, "Unknown %s\n", argv[1]);
        return 1;
    }

    return 0;
}

static int isJack(const char *fn)
{
    const char *inExt = "jack";

    const char *ext = strrchr(fn, '.');

    return ext != NULL && strcmp(ext+1, inExt) == 0;
}

static void changeExt(char *fn)
{
    const char *outExt = "TP.xml";
    char *ext = strrchr(fn, '.');

    if (ext == NULL) {
        fprintf(stderr, "Unknown %s\n", fn);
        return;
    }

    *ext = '\0';
    strcat(fn, outExt);
}

static fileIO fileProcess(const char *infn, const char *outfn)
{
    FILE *fin, *fout;

    if ((fin = fopen(infn, "r")) == NULL) {
        perror(infn);
        return READ_FAIL;
    }

    if ((fout = fopen(outfn, "w")) == NULL) {
        perror(outfn);
        fclose(fin);
        return WRITE_FAIL;
    }

    tokenize(fin, fout);

    fclose(fin);
    fclose(fout);

    return SUCCESS;
}