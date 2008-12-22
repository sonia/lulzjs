#include "Preprocessor.h"

const char*
preprocess (const char* source, const char* fileName)
{
#ifdef DEBUG
    printf("Preprocessing %s\n", fileName);
#endif
    if (source == NULL)
        return NULL;

    int line = 0;

    size_t sourceLength = strlen(source);

    char*  finalSource = malloc(1*sizeof(char));finalSource[0]='\0';
    size_t finalLength = 0;

    char*  includeName = NULL;
    size_t includeLength  = 0;

    size_t i;
    for (i = 0; i < sourceLength; i++) {
        if (source[i] == '\n' || i == 0) {
            line++;

            if (source[i] == '\n') {
                finalSource = realloc(finalSource, (++finalLength)*sizeof(char));
                finalSource[finalLength-1] = '\n';
                i++;
            }

            if (source[i] == '#') {
                i++;
                if (strstr(&source[i], "include") == &source[i]) {
                    i += 7;

                    while (i < sourceLength && source[i] != '"' && source[i] != '<' && source[i] != '\n')
                        i++;

                    includeName = NULL; includeLength = 0;
                    if (source[i] == '"' || source[i] == '<') {
                        i++;
                        while (i < sourceLength && source[i] != '"' && source[i] != '>' && source[i] != '\n') {
                            includeName = realloc(includeName, (++includeLength)*sizeof(char));
                            includeName[includeLength-1] = source[i];
                            i++;
                        }
                        includeName = realloc(includeName, (++includeLength)*sizeof(char));
                        includeName[includeLength-1] = '\0';
                        i++;
#ifdef DEBUG
                        printf("%s :O\n", fileName);
#endif

                        char* included = (char*) include(fileName, includeName, (source[i-1]=='"'?LOCAL:GLOBAL));
                        if (included == NULL) {
                            fprintf(stderr, "Included file (%s) on line %d couldn't be found.\n", includeName, line);
                        }
                        else if (included == (char*)-1) {
                            free(includeName);
                        }
                        else {
                            finalSource = realloc(finalSource, (finalLength+=strlen(included))*sizeof(char));
                            strcat(finalSource, included);
                            free(included);
                            free(includeName);
                        }
#ifdef DEBUG
                        printf("%s :O :O\n", fileName);
#endif
                    }
                }
            }
        }
        finalSource = realloc(finalSource, (++finalLength)*sizeof(char));
        finalSource[finalLength-1] = source[i];
    }
    
#ifdef DEBUG
    printf("%s:\n----------\n%s\n-----------\n", fileName, finalSource);
#endif

    return finalSource;
}

const char*
include (const char* from, const char* fileName, int type)
{
#ifdef DEBUG
    printf("Including %s from %s\n", fileName, from);
#endif

    char* path = NULL;
    if (type == LOCAL) {
        // Getting the dirname of the file from the other file is included
        // then copying it and getting the path to the dir.
        char* file = strdup(from);
        char* dir  = dirname(file);
        char* base = malloc((strlen(dir)+1)*sizeof(char));
        strcpy(base, dir); strcat(base, "/");

        // Copying the base to the path and then adding the relative path to
        // the file to import
        path = malloc(strlen(base)*sizeof(char));
        strcpy(path, base);
        path = realloc(path, (strlen(path)+strlen(fileName))*sizeof(char));
        strcat(path, fileName);
        free(file); free(base);
    }
    else {
        
    }

    return import(path);
}

const char*
import (const char* path)
{
    if (strstr(path, ".js") == &path[strlen(path)-3]) {
        return preprocess(readFile(path), path);
    }
    else {
        return (char*)-1;
    }
}
