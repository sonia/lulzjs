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
Interactive_start (JSContext* cx, JSObject* global)
{
    JSBool exit = JS_FALSE;

    JSScript* script;
    jsval     result;
    JSString* strResult;

    char* whole;
    char* line;

    int startline;
    int lineno;

    do {
        startline = lineno = 0;
        whole = NULL;

        do {
            printf("%s", startline == lineno ? ">>> " : "");
            
            line = readLine();

            if (startline == lineno) {
                if (strcmp(line, "quit") == 0) {
                    puts("GTFO");
                    return;
                }
            }

            whole = realloc(whole, (whole==NULL?0:strlen(whole))+strlen(line)*sizeof(char)+1);

            if (lineno == 0) {
                strcpy(whole, line);
            }
            else {
                strcat(whole, line);
            }

            free(line);
            lineno++;
        } while (!JS_BufferIsCompilableUnit(cx, global, whole, strlen(whole)));

        JS_ClearPendingException(cx);
        script = JS_CompileScript(cx, global, whole, strlen(whole), "lulzJS", startline);

        if (script) {
            if (JS_ExecuteScript(cx, global, script, &result)) {
                strResult = JS_ValueToString(cx, result);

                if (strResult) {
                    printf("<<< %s\n", JS_GetStringBytes(strResult));
                }
            }
            JS_DestroyScript(cx, script);
        }
        
        free(whole);
    } while (!exit);
}

