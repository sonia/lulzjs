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

#include "Socket.h"

short exec (JSContext* cx) { return Socket_initialize(cx); }

short
Socket_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JS_GetProperty(cx, JSVAL_TO_OBJECT(jsParent), "IO", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    jsval jsSuper;
    JS_GetProperty(cx, parent, "Stream", &jsSuper);

    JSObject* object = JS_InitClass(
        cx, parent, JSVAL_TO_OBJECT(jsSuper), &Socket_class,
        Socket_constructor, 2, NULL, Socket_methods, NULL, Socket_static_methods
    );

    if (!object)
        return 0;

    // Default properties
    jsval property;

    return 1;
}

void
Socket_finalize (JSContext* cx, JSObject* object)
{
    SocketInformation* data = JS_GetPrivate(cx, object);

    if (data) {

        free(data);
    }
}

JSBool
Socket_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    const char* fileName;
    const char* mode;

    if (argc != 2 || !JS_ConvertArguments(cx, argc, argv, "ss", &fileName, &mode)) {
        JS_ReportError(cx, "Socket requires the path and the mode as arguments.");
        return JS_FALSE;
    }

    SocketInformation* data = malloc(sizeof(SocketInformation));
    JS_SetPrivate(cx, object, data);

    return JS_TRUE;
}

JSBool
Socket_write (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    char* string;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "s", &string)) {
        return JS_FALSE;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    return JS_TRUE;
}

JSBool
Socket_read (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int size;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "u", &size)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    SocketInformation* data = JS_GetPrivate(cx, object);

    return JS_TRUE;
}

