#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>

#include "config.h"
#include "parser.h"
#include "codeWriter.h"

#define EXTIN   ".vm"
#define EXTOUT  ".asm"

static void setFileName(const char *);

char g_filename[PATH_MAX];

int main(int argc, char *argv[])
{
    struct stat st;
    DIR *dir;
    struct dirent *entry;
    char fnamein[BUF_MAX][PATH_MAX], fnameout[PATH_MAX];
    char *ext;
    int fileCount = 0;
    size_t len;
    FILE *fin, *fout;
    char buf[PATH_MAX];

    if (argc != 2) {
        fprintf(stderr, "Usage: VMTranslator source(.vm)\n");
        exit(1);
    }

    if (stat(argv[1], &st) == -1) {
        perror("stat");
        return 1;
    }

    if (S_ISDIR(st.st_mode)) {
        if ((dir = opendir(argv[1])) == NULL) {
            perror("argv[1]");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            // ignore "." ".."
            if (!strcmp(entry->d_name, ".")
                || !strcmp(entry->d_name, ".."))
                continue;

            ext = strrchr(entry->d_name, '.');

            // choose ".vm"
            if (ext != NULL && !strcmp(ext, EXTIN)) {
                if (fileCount >= BUF_MAX) {
                    fprintf(stderr, "Too many VM files.\n");
                    closedir(dir);
                    return 1;
                }

                len = strlen(argv[1]);

                if (len > 0 && argv[1][len-1] != '/')
                    snprintf(fnamein[fileCount], PATH_MAX,
                            "%s/%s", argv[1], entry->d_name);
                else 
                    snprintf(fnamein[fileCount], PATH_MAX,
                            "%s%s", argv[1], entry->d_name);

                fileCount++;
            }
        }

        closedir(dir);

        strcpy(buf, argv[1]);
        len = strlen(buf);

        if (buf[len-1] == '/')
            buf[len-1] = '\0';

        strcat(buf, EXTOUT);
        strcpy(fnameout, buf);
    } else if (S_ISREG(st.st_mode)) {
        strcpy(fnameout, argv[1]);
        ext = strrchr(fnameout, '.');

        if (ext == NULL || strcmp(ext, EXTIN)) {
            fprintf(stderr, "Usage: VMTranslator source.vm\n");
            exit(1);
        }

        *ext = '\0';
    
        strcat(fnameout, EXTOUT);
        strcpy(fnamein[fileCount], argv[1]);
        fileCount++;
    } else {
        fprintf(stderr, "Unknown argv[1]\n");
        return 1;
    }

    if (fileCount == 0) {
        fprintf(stderr, "No .vm files found\n");
        return 1;
    }

    if ((fout = fopen(fnameout, "w")) == NULL) {
        perror(fnameout);
        exit(1);
    }

    if (S_ISDIR(st.st_mode))
        writeBootstrap(fout);

    for (int i = 0; i < fileCount; i++) {
        if ((fin = fopen(fnamein[i], "r")) == NULL) {
            perror(fnamein[i]);
            fclose(fout);
            exit(1);
        }

        setFileName(fnamein[i]);

        parser(fin, fout);

        fclose(fin);
    }

    fclose(fout);

    return 0;
}

static void setFileName(const char *fn)
{
    char *base = strrchr(fn, '/');

    if (base != NULL)
        strcpy(g_filename, base+1);
    else
        strcpy(g_filename, fn);

    char *ext = strrchr(g_filename, '.');

    if (ext != NULL)
        *ext = '\0';
}