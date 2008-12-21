#include <stdio.h>
#include <js/jsapi.h>

#include "lib/Core.h"

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
    engine.error = JS_TRUE;

    if (engine.runtime = JS_NewRuntime(8L * 1024L * 1024L)) {
        if (engine.context = JS_NewContext(engine.runtime, 8192)) {
            JS_SetOptions(engine.context, JSOPTION_VAROBJFIX);
            JS_SetErrorReporter(engine.context, reportError);

            if (engine.core = Core_initialize(engine.context)) {
                engine.error = JS_FALSE;
                return engine;
            }
        }
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

