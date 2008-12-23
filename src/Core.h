#include <js/jsapi.h>

static JSClass Core_class = {
    "Core", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

JSObject* Core_initialize (JSContext *context);

static JSFunctionSpec Core_methods[] = {
    {NULL}
};

