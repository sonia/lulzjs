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

    int startline;
    int lineno;

    puts("lulzJS " __LJS_VERSION__ "\n");

    do {
        startline = lineno = 0;
        whole = NULL;

        do {
            line = readline(startline == lineno ? ">>> " : "");

            if (startline == lineno) {
                if (line == NULL || strcmp(line, "quit") == 0) {
                    puts("GTFO");
                    return;
                }
            }

            whole = JS_realloc(cx, whole, (whole==NULL?0:strlen(whole))+strlen(line)*sizeof(char)+1);

            if (lineno == 0) {
                strcpy(whole, line);
            }
            else {
                strcat(whole, line);
            }

            JS_free(cx, line);
            lineno++;
        } while (!JS_BufferIsCompilableUnit(cx, global, whole, strlen(whole)));

        if (strlen(whole) == 0) {
            continue;
        }

        add_history(whole);

        JS_ClearPendingException(cx);
        script = JS_CompileScript(cx, global, whole, strlen(whole), "lulzJS", startline);

        if (script) {
            if (JS_ExecuteScript(cx, global, script, &result)) {
                strResult = JS_ValueToString(cx, result);

                if (strResult && strcmp(strResult, "undefined") != 0) {
                    printf("<<< %s\n", JS_GetStringBytes(strResult));
                }
            }
            JS_DestroyScript(cx, script);
        }
        
        JS_free(cx, whole);
    } while (!exit);
}

