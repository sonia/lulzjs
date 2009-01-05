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

#include "System.h"

short exec (JSContext* cx) { return System_initialize(cx); }

short
System_initialize (JSContext* cx)
{
    JSObject* object = JS_DefineObject(
        cx, JS_GetGlobalObject(cx),
        System_class.name, &System_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (!object)
        return 0;

    JS_DefineFunctions(cx, object, System_methods);

    return 1;
}

JSBool
System_exec (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    FILE* pipe;
    char* output  = NULL;
    size_t length = 0;
    size_t read   = 0;
    const char* command;

    if (!JS_ConvertArguments(cx, argc, argv, "s", &command)) {
        return JS_FALSE;
    }
    if (argc != 1) {
        JS_ReportError(cx, "exec needs 1 arguments");
        return JS_FALSE;
    }

    if ((pipe = popen(command, "r")) == NULL) {
        JS_ReportError(cx, "command not found");
        return JS_FALSE;
    }

    while (1) {
        output = JS_realloc(cx, output, length+=512);
        read   = fread(output+(length-512), sizeof(char), 512, pipe);

        if (read < 512) {
            output = JS_realloc(cx, output, length-=(512-read));
            break;
        }
    }
    output[length-1] = '\0';
    pclose(pipe);

    *rval = STRING_TO_JSVAL(JS_NewString(cx, output, length));
    return JS_TRUE;
}

