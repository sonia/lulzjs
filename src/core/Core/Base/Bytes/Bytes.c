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
    jsval class;
    jsval detach = JSVAL_FALSE;

    if (argc < 1) {
        JS_ReportError(cx, "Bytes needs an object to transform into a thread.");
        return JS_FALSE;
    }

    switch (argc) {
        case 2: detach = argv[1]; break;
        case 1: class  = argv[0];  break;
    }

    pthread_t* thread = JS_malloc(cx, sizeof(pthread_t));
    JS_SetPrivate(cx, object, thread);

    JS_SetProperty(cx, object, "__class", &class);
    JS_SetProperty(cx, object, "__detach", &detach);

    return JS_TRUE;
}

void
Bytes_finalize (JSContext* cx, JSObject* object)
{
    pthread_t* thread = JS_GetPrivate(cx, object);

    if (thread) {
        JS_free(cx, thread);
    }
}