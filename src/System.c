#include "System.h"

JSObject*
System_initialize (JSContext* context)
{
    JSObject* object = JS_NewObject(context, &System_class, NULL, NULL);
    if (!object) {
        return NULL;
    }

    if (!JS_InitStandardClasses(context, object)) {
        return NULL;
    }

    JS_DefineProperty(context, object, "STDIN",  INT_TO_JSVAL(stdin),  NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDOUT", INT_TO_JSVAL(stdout), NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDERR", INT_TO_JSVAL(stderr), NULL, NULL, JSPROP_READONLY);

    return object;
}

JSBool
System_write (JSContext *context, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    const FILE  buffer;
    const char* string;

    if (!JS_ConvertArguments(context, argc, argv, "us", &buffer, &string)) {
        return JS_FALSE;
    }

    if (argc != 2) {
        return JS_TRUE;
    }

    *rval = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), &buffer));
    return JS_TRUE;
}

