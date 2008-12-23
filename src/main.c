#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <js/jsapi.h>

#include "Misc.h"
#include "Preprocessor.h"
#include "Core.h"

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
main (int argc, char *argv[])
{
    int cmd;
    while ((cmd = getopt(argc, argv, "v")) != -1) {
    }

    if (argc < 1 || !fileExists(argv[optind])) {
        fprintf(stderr, "You have to pass a source.\n");
        return 1;
    }

    Engine engine = initEngine();
    if (engine.error) {
        fprintf(stderr, "An error occurred while initializing the system.\n");
        return 1;
    }

    if (!executeScript(engine.context, argv[optind])) {
        fprintf(stderr, "The script couldn't be executed.\n");
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
    JSBool    returnValue;
    jsval     rval;
    JSObject* global = JS_GetGlobalObject(context);

    JSScript* script;
    char* sources = (char*) preprocess(context, readFile(file), file);

    if (!sources) {
        return JS_FALSE;
    }

    returnValue = JS_EvaluateScript(context, global, sources, strlen(sources), file, 0, &rval);
    free(sources);
    return returnValue;
}

