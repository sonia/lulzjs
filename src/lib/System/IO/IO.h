#include <js/jsapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static JSClass IO_class = {
    "IO", 0,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub
};

extern void      exec (JSContext* context);
JSObject* IO_initialize (JSContext* context);
JSBool    IO_write (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);

static JSFunctionSpec IO_methods[] = {
    {"write", IO_write, 0, 0, 0},
    {NULL}
};
