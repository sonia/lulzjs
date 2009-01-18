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

#include "Bytes.h"

JSBool exec (JSContext* cx) { return Bytes_initialize(cx); }

JSBool
Bytes_initialize (JSContext* cx)
{
    JSObject* parent = JS_GetGlobalObject(cx);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Bytes_class,
        Bytes_constructor, 1, NULL, Bytes_methods, NULL, Bytes_static_methods
    );

    if (object) {
        
        return JS_TRUE;
    }

    return JS_FALSE;
}

JSBool
Bytes_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* array;

    if (argc < 1 || !JS_ConvertArguments(cx, argc, argv, "o", &array)) {
        JS_ReportError(cx, "Not enough parameters.");
        return JS_FALSE;
    }

    if (!JS_IsArrayObject(cx, array)) {
        JS_ReportError(cx, "Bytes needs an array of ints.");
        return JS_FALSE;
    }

    jsval property = OBJECT_TO_JSVAL(array);
    JS_SetProperty(cx, object, "__array", &property);

    return JS_TRUE;
}

void
Bytes_finalize (JSContext* cx, JSObject* object)
{
}
