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

#include "Interactive.h"

void
Interactive (JSContext* cx, JSObject* global)
{
    JSBool exit = JS_FALSE;

    JSScript* script;
    jsval     result;
    JSString* strResult;

    char* whole;
    char* line;
    int lineNumber = 0;

    int startline;
    int lineno;

    puts("lulzJS " __LJS_VERSION__ "\n");

    do {
        startline = lineno = 1;
        whole = NULL;

        do {
            line = readline(startline == lineno ? ">>> " : "... ");

            // If it's the first line and nothing has been written or the line
            // is quit exit the interactive mode.
            if (startline == lineno) {
                if (line == NULL || strcmp(line, "quit") == 0) {
                    puts("GTFO");
                    return;
                }
            }

            // Realloc the whole string with length of whole + the length of the inserted line.
            whole = JS_realloc(cx, whole, (whole==NULL?0:strlen(whole))+strlen(line)*sizeof(char)+1);

            // If it's the first line use strcpy to avoid not cleaned memory bugs if it's not
            // use strcat.
            whole = (lineno == 1) ? strcpy(whole, line) : strcat(whole, line);

            JS_free(cx, line);
            lineno++; lineNumber++;
        } while (!JS_BufferIsCompilableUnit(cx, global, whole, strlen(whole)));

        if (strlen(whole) == 0) {
            continue;
        }

        add_history(whole);

        JS_ClearPendingException(cx);
        script = JS_CompileScript(cx, global, whole, strlen(whole), "lulzJS", lineNumber);

        if (script) {
            if (JS_ExecuteScript(cx, global, script, &result)) {
                jsval val = result;
                jsval jsObj; JS_GetProperty(cx, global, "Object", &jsObj);
                JSObject* object = JSVAL_TO_OBJECT(jsObj);

                jsval argv[] = {val};
                JS_CallFunctionName(cx, object, "inspect", 1, argv, &result);

                strResult = JS_ValueToString(cx, result);

                if (strResult) {
                    char* str = JS_GetStringBytes(strResult);

                    if (strcmp(str, "undefined") != 0) {
                        printf("%s\n", JS_GetStringBytes(strResult));
                    }
                }
            }
            JS_DestroyScript(cx, script);
        }
        
        JS_free(cx, whole);
    } while (!exit);
}

