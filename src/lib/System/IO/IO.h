#include <js/jsapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static JSClass IO_class = {
    "IO", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

extern short  exec (JSContext* context);
extern short  IO_initialize (JSContext* context);
extern JSBool IO_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSFunctionSpec IO_methods[] = {
    {"write", IO_write, 0, 0, 0},
    {NULL}
};
