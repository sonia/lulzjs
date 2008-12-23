#include "System.h"

short exec (JSContext* context) { return System_initialize(context); }

short
System_initialize (JSContext* context)
{
    JSObject* object = JS_DefineObject(
        context, JS_GetGlobalObject(context),
        System_class.name, &System_class, NULL, 
        JSPROP_PERMANENT|JSPROP_READONLY|JSPROP_ENUMERATE);

    if (!object)
        return 0;

    JS_DefineFunctions(context, object, System_methods);

    return 1;
}

