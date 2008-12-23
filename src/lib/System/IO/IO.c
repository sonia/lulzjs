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

#include "IO.h"

short exec (JSContext* context) { return IO_initialize(context); }

short
IO_initialize (JSContext* context)
{
    jsval jsParent;
    JS_GetProperty(context, JS_GetGlobalObject(context), "System", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_DefineObject(
        context, parent,
        IO_class.name, &IO_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (!object)
        return 0;

    JS_DefineProperty(context, object, "STDIN",  INT_TO_JSVAL(fileno(stdin)),  NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDOUT", INT_TO_JSVAL(fileno(stdout)), NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDERR", INT_TO_JSVAL(fileno(stderr)), NULL, NULL, JSPROP_READONLY);

    JS_DefineFunctions(context, object, IO_methods);

    return 1;
}

JSBool
IO_write (JSContext *context, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int fd;
    const char*        string;

    if (!JS_ConvertArguments(context, argc, argv, "us", &fd, &string)) {
        return JS_FALSE;
    }
    if (argc != 2) {
        return JS_TRUE;
    }

    FILE* fp;
    switch (fd) {
        case  0: fp = stdin;            break;
        case  1: fp = stdout;           break;
        case  2: fp = stderr;           break;
        default: fp = fdopen(fd, "r+"); break;
    }

    *rval = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), fp));
    return JS_TRUE;
}

JSBool
IO_read (JSContext *context, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int fd;
    const unsigned int size;

    if (!JS_ConvertArguments(context, argc, argv, "uu", &fd, &size)) {
        return JS_FALSE;
    }
    if (argc != 2) {
        return JS_TRUE;
    }

    char* string = malloc(size*sizeof(char));

    FILE* fp;
    switch (fd) {
        case  0: fp = stdin;            break;
        case  1: fp = stdout;           break;
        case  2: fp = stderr;           break;
        default: fp = fdopen(fd, "r+"); break;
    }

    fread(string, sizeof(char), size, fp);
    *rval = STRING_TO_JSVAL(JS_NewString(context, string, size));
    return JS_TRUE;
}

