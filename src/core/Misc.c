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

#include "Misc.h"

char*
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
            text = realloc(text, length-=(512-read));
            break;
        }
    }
    text[length-1] = '\0';

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

char*
stripComments (char* text)
{
    size_t textLength = strlen(text);

    char* stripped = NULL;
    size_t length  = 0;
    short newLine  = 1;

    size_t i;
    for (i = 0; i < textLength; i++) {
        if (newLine) {
            newLine = 0;
            if (text[i] == '#') {
                while (i < textLength && text[i] != '\n') {
                    i++;
                }
                i++;
                newLine = 1;
            }
        }

        stripped = realloc(stripped, (++length)*sizeof(char));
        stripped[length-1] = text[i];
        
        if (text[i] == '\n')
            newLine = 1;
    }
    stripped = realloc(stripped, (++length)*sizeof(char));
    stripped[length-1] = '\0';
    free(text);

    return stripped;
}
