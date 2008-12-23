#include <js/jsapi.h>

static JSClass System_class = {
    "System", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

extern void exec (JSContext* context);
extern JSObject* System_initialize (JSContext* context);
extern JSBool    System_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSFunctionSpec System_methods[] = {
    {NULL}
};
