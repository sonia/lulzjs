#include "Misc.h"

const char*
readFile (const char* file)
{
    FILE*  fp     = fopen(file, "r");
    char*  text   = NULL;
    size_t length = 0;
    size_t read   = 0;

    if (!fp) {
        return NULL;
    }

    while (1) {
        text = realloc(text, length+=512);
        read = fread(text+(length-512), sizeof(char), 512, fp);

        if (read < 512) {
            text = realloc(text, length-=(512-read-1));
            break;
        }
    }
    fclose(fp);

    return text;
}

short
fileExists (const char* file)
{
    FILE* check = fopen(file, "r");

    if (check) {
        fclose(check);
        return 1;
    }
    else {
        return 0;
    }
}

