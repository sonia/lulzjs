#include "System.h"

JSObject*
System_initialize (JSContext* context)
{
    JSObject* object = JS_DefineObject(
        context, JS_GetGlobalObject(context),
        System_class.name, &System_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (!object) {
        return NULL;
    }

    JS_DefineProperty(context, object, "STDIN",  INT_TO_JSVAL(fileno(stdin)),  NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDOUT", INT_TO_JSVAL(fileno(stdout)), NULL, NULL, JSPROP_READONLY);
    JS_DefineProperty(context, object, "STDERR", INT_TO_JSVAL(fileno(stderr)), NULL, NULL, JSPROP_READONLY);

    JS_DefineFunctions(context, object, System_methods);

    return object;
}

JSBool
System_write (JSContext *context, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    const unsigned int buffer;
    const char*        string;

    if (!JS_ConvertArguments(context, argc, argv, "us", &buffer, &string)) {
        return JS_FALSE;
    }
    if (argc != 2) {
        return JS_TRUE;
    }

    *rval = INT_TO_JSVAL(fwrite(string, sizeof(*string), strlen(string), fdopen(buffer, "r+")));
    return JS_TRUE;
}

