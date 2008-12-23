#include "System.h"

void exec (JSContext* context) { System_initialize(context); }

JSObject*
System_initialize (JSContext* context)
{
    JSObject* object = JS_DefineObject(
        context, JS_GetGlobalObject(context),
        System_class.name, &System_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (!object)
        return NULL;

    JS_DefineFunctions(context, object, System_methods);

    return object;
}

