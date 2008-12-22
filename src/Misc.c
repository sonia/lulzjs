#include "Misc.h"

const char*
readFile (const char* file)
{
    FILE*  file = fopen(file, "r");
    char*  text;
    size_t length;
    size_t read;

    if (!file) {
        return NULL;
    }

    while (1) {
        text = realloc(text, (length+=512));
        read = fread(text+(length-512), sizeof(char), 512, file);

        if (read < 512) {
            text = realloc(text, length-=(512-read));
            break;
        }
    }
    fclose(file);

    return text;
}
