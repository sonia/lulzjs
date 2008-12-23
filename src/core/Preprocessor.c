/****************************************************************************
* This file is part of lulzJS                                               *
* Copyleft meh.                                                             *
*                                                                           *
* lulzJS is free software: you can redistribute it and/or modify            *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation, either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* lulzJS is distributed in the hope that it will be useful.                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty o.            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See th.             *
* GNU General Public License for more details.                              *
*                                                                           *
* You should have received a copy of the GNU General Public License         *
* along with lulzJS.  If not, see <http://www.gnu.org/licenses/>.           *
****************************************************************************/

#include "Preprocessor.h"

const char*
preprocess (JSContext* context, const char* source, const char* fileName)
{
#ifdef DEBUG
    printf("Preprocessing %s\n", fileName);
#endif

    int line      = 0;
    short newLine = 1;

    size_t sourceLength = strlen(source);

    char*  finalSource = malloc(1*sizeof(char));finalSource[0]='\0';
    size_t finalLength = 1;

    char*  includeName   = NULL;
    size_t includeLength = 0;

    size_t i;
    for (i = 0; i < sourceLength; i++) {
        if (newLine) {
            newLine = 0;
            line++;

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

                        if (!include(context, fileName, includeName, (source[i-1]=='"'?LOCAL:GLOBAL))) {
                            fprintf(stderr, "%s:%d > %s not found.\n", fileName, line, includeName);
                            return NULL;
                        }
                    }
                }
                else {
                    while (i < sourceLength && source[i] != '\n')
                        i++;
                }
            }
        }
        finalSource = realloc(finalSource, (++finalLength)*sizeof(char));
        finalSource[finalLength-2] = source[i]; finalSource[finalLength-1] = '\0';

        if (source[i] == '\n') {
            newLine = 1;
        }
    }

    return finalSource;
}

short
include (JSContext* context, const char* from, const char* fileName, int type)
{
#ifdef DEBUG
    printf("Including %s from %s\n", fileName, from);
#endif

    char* path = NULL;

    if (type == LOCAL) {
        /* Getting the dirname of the file from the other file is included
         * then copying it and getting the path to the dir.
         */
        char* file = strdup(from);
        char* dir  = dirname(file);
        char* base = malloc((strlen(dir)+1)*sizeof(char));
        strcpy(base, dir); strcat(base, "/");
    
        /* Copying the base to the path and then adding the relative path to
         * the file to import
         */
        path = strdup(base);
        path = realloc(path, (strlen(path)+strlen(fileName))*sizeof(char));
        strcat(path, fileName);
        free(base);
    }
    else {
        path = strdup(__LJS_LIBRARY_PATH__);
        path = realloc(path, strlen(path)+strlen(fileName));
        strcat(path, fileName);
    }

    return import(context, path);
}

// TODO: Keep track of imported stuff.
short
import (JSContext* context, const char* path)
{

    if (!fileExists(path)) {
        return 0;
    }

    if (strstr(path, ".js") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(javascript) path: %s\n", path);
        #endif

        jsval rval;
        char* sources = (char*)preprocess(context, readFile(path), path);
        JS_EvaluateScript(context, JS_GetGlobalObject(context), sources, strlen(sources), path, 0, &rval);
        free(sources);
    }
    else if (strstr(path, ".so") == &path[strlen(path)-3]) {
        #ifdef DEBUG
        printf("(object) path: %s\n", path);
        #endif

        void* handle = dlopen(path, RTLD_LAZY|RTLD_GLOBAL);

        #ifdef DEBUG
        printf("handle: %d [%s]\n", handle, dlerror());
        #endif

        short (*exec)(JSContext*) = dlsym(handle, "exec");

        #ifdef DEBUG
        printf("function: %d [%s]\n", exec, dlerror());
        #endif

        if(!(*exec)(context)) {
            return 0;
        }

        #ifdef DEBUG
        printf("executed\n");
        #endif
    }
    else {
        #ifdef DEBUG
        printf("(module) path: %s\n", path);
        #endif

        char* newPath = strdup(path);
        newPath = realloc(newPath, strlen(newPath)+strlen("/init.js"));
        strcat(newPath, "/init.js");

        if (!fileExists(newPath))
            return 0;
        
        import(context, newPath);
        free(newPath);
    }

    return 1;
}
