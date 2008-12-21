#include <stdio.h>
#include <js/jsapi.h>

#include "System.h"

static JSClass Core_class = {
    "Core", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

typedef struct {
    JSBool     error;
    JSRuntime* runtime;
    JSContext* context;
    JSObject*  core;
} Engine;

void reportError (JSContext *cx, const char *message, JSErrorReport *report);
Engine initEngine (void);
JSBool executeScript (JSContext* context, const char* file);

int
main (int argc, const char *argv[])
{
    Engine engine = initEngine();
    if (engine.error) {
        fprintf(stderr, "An error occurred while initializing the system.\n");
        return 1;
    }
    if (!System_initialize(engine.context)) {
        fprintf(stderr, "The System object doesn't exist.\n");
        return 1;
    }
    if (!executeScript(engine.context, argv[1])) {
        return 1;
    }

    JS_DestroyContext(engine.context);
    JS_DestroyRuntime(engine.runtime);
    JS_ShutDown();
    return 0;
}

void
reportError (JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u > %s\n",
            report->filename ? report->filename : "<no filename>",
            (unsigned int) report->lineno,
            message);
}

Engine
initEngine (void)
{
    Engine engine;
    engine.error = JS_FALSE;

    engine.runtime = JS_NewRuntime(8L * 1024L * 1024L);
    if (!engine.runtime) {
        engine.error = JS_TRUE;
        return engine;
    }

    engine.context = JS_NewContext(engine.runtime, 8192);
    if (!engine.context) {
        engine.error = JS_TRUE;
        return engine;
    }
    JS_SetOptions(engine.context, JSOPTION_VAROBJFIX);
    JS_SetErrorReporter(engine.context, reportError);

    engine.core = JS_NewObject(engine.context, &Core_class, NULL, NULL);
    if (!engine.core) {
        engine.error = JS_TRUE;
        return engine;
    }
    if (!JS_InitStandardClasses(engine.context, engine.core)) {
        engine.error = JS_TRUE;
        return engine;
    }

    return engine;
}

JSBool
executeScript (JSContext* context, const char* file)
{
    JSScript* script;
    jsval     returnValue;
    JSObject* global = JS_GetGlobalObject(context);

    script = JS_CompileFile(context, global, file);
    if (!script) {
        return JS_FALSE;
    }

    return JS_ExecuteScript(context, global, script, &returnValue);
}

