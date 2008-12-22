#include "Preprocessor.h"

const char*
preprocessSource (const char* source)
{
    char* include = NULL;

    size_t i;
    for (i = 0; i < strlen(source); i++) {
        if (source[i] == '\n') {
            if (source[i+1] == '#') {
                i += 2;
                if (strstr(source[i], "include") == source[i]) {
                    i += 7;
                    if (source[i] == '"' || source[i] == '<') {

                    }
                }
            }
        }
    }
}
