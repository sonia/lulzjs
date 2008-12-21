#include "Core.h"

JSObject*
Core_initialize (JSContext *context)
{
    JSObject* object = JS_NewObject(context, &Core_class, NULL, NULL);

    if (object && JS_InitStandardClasses(context, object))
            return object;

    return NULL;
}
