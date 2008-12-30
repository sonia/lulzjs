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

#include "Stream.h"

extern short exec (JSContext* cx) { return Stream_initialize(cx); }

short
Stream_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(jsParent), "IO", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Stream_class,
        NULL, 0, NULL, Stream_methods, NULL, NULL 
    );

    if (!object)
        return 0;

    JSObject* stream;
    StreamInformation* data;

    // Create STDIN special Stream object.
    stream = JS_DefineObject(
        cx, parent,
        "STDIN", &Stream_class, object,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );
    data = (StreamInformation*) malloc(sizeof(StreamInformation));
    data->descriptor = stdin;
    JS_SetPrivate(cx, stream, data);

    // Create STDOUT special Stream object.
    stream = JS_DefineObject(
        cx, parent,
        "STDOUT", &Stream_class, object,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );
    data = (StreamInformation*) malloc(sizeof(StreamInformation));
    data->descriptor = stdout;
    JS_SetPrivate(cx, stream, data);
  
    // Create STDERR special stream object.
    stream = JS_DefineObject(
        cx, parent,
        "STDERR", &Stream_class, object,
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE
    );
    data = (StreamInformation*) malloc(sizeof(StreamInformation));
    data->descriptor = stderr;
    JS_SetPrivate(cx, stream, data);

    // Default properties
    jsval property;

//    property = INT_TO_JSVAL(EOF);
//    JS_SetProperty(cx, parent, "EOF", property);

    return 1;
}

JSBool
Stream_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    StreamInformation* data = JS_GetPrivate(cx, object);

    *rval = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), data->descriptor));
    return JS_TRUE;
}

JSBool
Stream_read (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int size;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    StreamInformation* data = (StreamInformation*) JS_GetPrivate(cx, object);

    if (feof(data->descriptor)) {
        *rval = JSVAL_NULL;
        return JS_TRUE;
    }

    char* string = malloc(size*sizeof(char));
    fread(string, sizeof(char), size, data->descriptor);

    *rval = STRING_TO_JSVAL(JS_NewString(cx, string, size));
    return JS_TRUE;
}

