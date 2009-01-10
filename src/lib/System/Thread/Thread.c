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

#include "Thread.h"

short exec (JSContext* cx) { return Thread_initialize(cx); }

short
Thread_initialize (JSContext* cx)
{
    jsval jsParent;
    JS_GetProperty(cx, JS_GetGlobalObject(cx), "System", &jsParent);
    JSObject* parent = JSVAL_TO_OBJECT(jsParent);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Thread_class,
        Thread_constructor, 1, NULL, Thread_methods, NULL, Thread_static_methods
    );

    if (!object)
        return 0;

    // Default properties
    jsval property;

    property = INT_TO_JSVAL(EOF);
    JS_SetProperty(cx, parent, "EOF", &property);

    return 1;
}

JSBool
Thread_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* class;

    if (argc != 2 || !JS_ConvertArguments(cx, argc, argv, "o", &class)) {
        JS_ReportError(cx, "Thread requires the path and the mode as arguments.");
        return JS_FALSE;
    }

    pthread_t* thread = JS_malloc(cx, sizeof(pthread_t));
    JS_SetPrivate(cx, object, thread);

    JS_DefineProperty(cx, object, "__class", OBJECT_TO_JSVAL(class), NULL, NULL, JSPROP_READONLY);

    return JS_TRUE;
}

void
Thread_finalize (JSContext* cx, JSObject* object)
{
    pthread_t* thread = JS_GetPrivate(cx, object);

    if (thread) {
        JS_free(cx, thread);
    }
}

JSBool
Thread_start (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    pthread_t* thread = JS_GetPrivate(cx, object);

    ThreadData* data = JS_malloc(cx, sizeof(ThreadData));
    data->cx   = cx;
    data->argc = argc;
    data->argv = argv;

    pthread_create(*thread, NULL, __Thread_start, data);
    pthread_detach(*thread);

    *rval = JSVAL_NULL;
    return JS_TRUE;
}

void*
__Thread_start (void* arg)
{
    ThreadData* data = (ThreadData*) arg;
    JSContext* cx   = data->cx;
    uintN      argc = data->argc;
    jsval*     argv = data->argv;
    JS_free(cx, data);
    
    // :O BOH
}

JSBool
Thread_stop (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    pthread_t* thread = JS_GetPrivate(cx, object);
    pthread_cancel(*thread);

    *rval = JSVAL_NULL;
    return JS_TRUE;
}

JSBool
Thread_static_cancel (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    JSObject* obj;

    if (argc != 1 || !JS_ConvertArguments(cx, argc, argv, "o", &obj)) {
        JS_ReportError(cx, "Not enough paramteres.");
        return JS_FALSE;
    }

    pthread_t *thread = JS_GetInstancePrivate(cx, obj, &Thread_class, NULL);

    if (!thread) {
        JS_ReportError(cx, "It's not a thread.");
        return JS_FALSE;
    }

    pthread_cancel(thread);

    return JS_TRUE;
}

JSBool
Thread_static_cancelPoint (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    pthread_testcancel();
}

