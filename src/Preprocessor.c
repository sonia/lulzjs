#include "Preprocessor.h"

const char*
preprocessSource (const char* source)
{
    size_t sourceLength = strlen(source);

    char*  finalSource = NULL;
    size_t finalLength = 0;

    char*  include = NULL;
    size_t length  = 0;

    size_t i;
    for (i = 0; i < sourceLength; i++) {
        if (source[i] == '\n' || i == 0) {
            if (source[source[i]=='\n'?++i:i] == '#') {
                i++;
                if (strstr(&source[i], "include") == &source[i]) {
                    i += 7;

                    while (i < sourceLength && source[i] != '"' && source[i] != '<' && source[i] != '\n')
                        i++;

                    include = NULL; length = 0;
                    if (source[i] == '"' || source[i] == '<') {
                        i++;
                        while (i < sourceLength && source[i] != '"' && source[i] != '>' && source[i] != '\n') {
                            include = realloc(include, ++length);
                            include[length-1] = source[i];
                            i++;
                        }
                        i++;

                        char* included = (char*) preprocessSource(readFile(include));
                        if (!included)
                            return NULL;

                        finalSource = realloc(finalSource, finalLength+=strlen(included));
                        strcat(finalSource, included);
                        free(included);
                    }
                }
            }
        }
        finalSource = realloc(finalSource, ++finalLength);
        finalSource[finalLength-1] = source[i];
    }
    
#ifdef DEBUG
    printf("%s\n", finalSource);
#endif

    return finalSource;
}
