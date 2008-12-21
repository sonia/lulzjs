#include <js/jsapi.h>
#include <string.h>

static JSClass System_class = {
    "System", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

JSObject* System_initialize (JSContext* context);
JSBool System_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSFunctionSpec System_methods[] = {
    {"write", System_write, 0, 0, 0},
    {NULL},
};

