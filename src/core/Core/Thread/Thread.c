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

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u > %s\n",
        "Thread",
        (unsigned int) report->lineno,
        message
    );
}

short
Thread_initialize (JSContext* cx)
{
    JSObject* parent = JS_GetGlobalObject(cx);

    JSObject* object = JS_InitClass(
        cx, parent, NULL, &Thread_class,
        Thread_constructor, 1, NULL, Thread_methods, NULL, Thread_static_methods
    );

    if (!object)
        return 0;

    // Default properties
    jsval property;

    return 1;
}

JSBool
Thread_constructor (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    jsval class;
    jsval detach = JSVAL_FALSE;

    if (argc < 1) {
        JS_ReportError(cx, "Thread needs an object to transform into a thread.");
        return JS_FALSE;
    }

    switch (argc) {
        case 2: detach = argv[1]; break;
        case 1: class  = argv[0];  break;
    }

    pthread_t* thread = JS_malloc(cx, sizeof(pthread_t));
    JS_SetPrivate(cx, object, thread);

    JS_SetProperty(cx, object, "class", &class);
    JS_SetProperty(cx, object, "__detach", &detach);

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

    ThreadData* data = malloc(sizeof(ThreadData));
    data->cx = JS_NewContext(JS_GetRuntime(cx), 8192);
    JS_SetOptions(data->cx, JSOPTION_VAROBJFIX);
    JS_SetErrorReporter(data->cx, reportError);
    JS_SetGlobalObject(data->cx, JS_GetGlobalObject(cx));

    data->object = object;
    data->argc   = argc;
    data->argv   = argv;

    jsval property; JS_GetProperty(cx, object, "__detach", &property);
    JSBool detach = JSVAL_TO_BOOLEAN(property);

    pthread_create(thread, NULL, __Thread_start, data);

    if (detach) {
        pthread_detach(*thread);
    }

    return JS_TRUE;
}

void*
__Thread_start (void* arg)
{
    ThreadData* data  = (ThreadData*) arg;
    JSContext* cx     = data->cx;
    JSObject*  object = data->object;
    uintN      argc   = data->argc;
    jsval*     argv   = data->argv;
    free(data);

    JS_BeginRequest(cx);
    jsval detach; JS_GetProperty(cx, object, "__detach", &detach);

    jsval property;
    JS_GetProperty(cx, object, "going", &property);

    if (!JSVAL_IS_VOID(property) && JSVAL_TO_BOOLEAN(property)) {
        return NULL;
    }

    property = JSVAL_TRUE; JS_SetProperty(cx, object, "going", &property);
    property = JSVAL_TRUE; JS_SetProperty(cx, object, "started", &property);

    // Get the class that's the actual class to construct.
    JS_GetProperty(cx, object, "class", &property);
    JSObject* class = JSVAL_TO_OBJECT(property);

    // Get the prototype of the object to use in the JS_ConstructObject
    jsval jsProto; JS_GetProperty(cx, class, "prototype", &jsProto);
    JSObject* proto = JSVAL_TO_OBJECT(jsProto);

    // Construct the object if it's a javascript thingy.
    JSObject* threadObj = JS_ConstructObject(cx, NULL, proto, NULL);
    property = OBJECT_TO_JSVAL(threadObj);
    JS_SetProperty(cx, object, "object", &property);

    // Execute the actual javascript constructor
    jsval ret;
    JS_CallFunctionValue(cx, threadObj, OBJECT_TO_JSVAL(class), argc, argv, &ret);

    property = JSVAL_FALSE;
    JS_SetProperty(cx, object, "going", &property);

    JS_EndRequest(cx);
    JS_DestroyContext(cx);

    if (!JSVAL_TO_BOOLEAN(detach)) {
        pthread_exit(&ret);
    }
}

JSBool
Thread_join (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    pthread_t* thread = JS_GetPrivate(cx, object);

    jsval ret;
    pthread_join(*thread, (void*) &ret);
    
    *rval = ret;

    return JS_TRUE;
}

JSBool
Thread_stop (JSContext *cx, JSObject *object, uintN argc, jsval *argv, jsval *rval)
{
    pthread_t* thread = JS_GetPrivate(cx, object);

    jsval property = JSVAL_FALSE;
    JS_SetProperty(cx, object, "going", &property);

    *rval = BOOLEAN_TO_JSVAL(pthread_cancel(*thread));
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

    *rval = BOOLEAN_TO_JSVAL(pthread_cancel(*thread));

    return JS_TRUE;
}

JSBool
Thread_static_cancelPoint (JSContext* cx, JSObject* object, uintN argc, jsval* argv, jsval* rval)
{
    pthread_testcancel();
}

